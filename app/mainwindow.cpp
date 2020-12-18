#include "mainwindow.h"
#include "run.hpp"
#include "ui_mainwindow.h"
#include <exception>

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  methodOptions = new QMap<QString, Options::Method>;
  functionOptions = new QMap<QString, Options::FunctionName>;
  stopCriterionOptions = new QMap<QString, Options::StopCriterion>;

  methodOptions->insert("Random search", Options::Method::random);
  methodOptions->insert("Nelder-Mead", Options::Method::nelder_mead);

  functionOptions->insert("Sphere", Options::FunctionName::sphere);
  functionOptions->insert("Easom", Options::FunctionName::easom);
  functionOptions->insert("Rastrigin", Options::FunctionName::rastrigin);
  functionOptions->insert("Rosenbrock", Options::FunctionName::rosenbrock);
  functionOptions->insert("Himmelblau", Options::FunctionName::himmelblau);

  stopCriterionOptions->insert("Minimum standard deviation",
                               Options::StopCriterion::min_std_dev);
  stopCriterionOptions->insert("Maximum iterations",
                               Options::StopCriterion::num_iterations);

  ui->plot->axisRect()->setupFullAxesBox(true);
  ui->plot->xAxis->setLabel("x");
  ui->plot->yAxis->setLabel("y");

  colorMap = new QCPColorMap(ui->plot->xAxis, ui->plot->yAxis);
  colorScale = new QCPColorScale(ui->plot);
  marginGroup = new QCPMarginGroup(ui->plot);
  int nx = 500;
  int ny = 500;
  colorMap->data()->setSize(nx, ny);
  colorMap->data()->setRange(QCPRange(-10, 10), QCPRange(-10, 10));
  ui->plot->plotLayout()->addElement(0, 1, colorScale);
  colorMap->setColorScale(colorScale);

  colorMap->setGradient(QCPColorGradient::gpPolar);
  colorScale->setType(QCPAxis::atRight);
  ui->plot->axisRect()->setMarginGroup(QCP::msBottom | QCP::msTop, marginGroup);
  colorScale->setMarginGroup(QCP::msBottom | QCP::msTop, marginGroup);

  QObject::connect(
    ui->plot, &QCustomPlot::mousePress, this, &MainWindow::on_plot_click);

  ui->plot->addLayer("points", ui->plot->layer("main"), QCustomPlot::limAbove);
  ui->plot->layer("points")->setMode(QCPLayer::lmBuffered);
  ui->plot->setCurrentLayer("points");
  ui->plot->addGraph();

  ui->plot->graph(0)->setScatterStyle(QCPScatterStyle::ssCircle);
  ui->plot->graph(0)->setPen(QPen(QBrush(Qt::red), 2));

  ui->method_comboBox->addItems(QStringList(methodOptions->keys()));
  ui->function_comboBox->addItems(QStringList(functionOptions->keys()));
}

MainWindow::~MainWindow()
{
  delete ui;
  delete methodOptions;
  delete functionOptions;
}

void
MainWindow::on_actionExit_triggered()
{
  QApplication::quit();
}

void
MainWindow::on_function_comboBox_currentIndexChanged(QString value)
{
  ui->plot->setCurrentLayer("points");
  ui->plot->graph(0)->data().data()->clear();

  int nx = 500;
  int ny = 500;

  double x, y;
  for (int xIndex = 0; xIndex < nx; ++xIndex) {
    for (int yIndex = 0; yIndex < ny; ++yIndex) {
      colorMap->data()->cellToCoord(xIndex, yIndex, &x, &y);
      double r =
        functions.at(functionOptions->value(value))(Eigen::Vector2d(x, y));
      colorMap->data()->setCell(xIndex, yIndex, r);
    }
  }

  colorMap->rescaleDataRange(true);
  colorMap->rescaleValueAxis();

  ui->plot->rescaleAxes();
  ui->plot->replot();
}

void
MainWindow::on_plot_click(QMouseEvent* event)
{
  if (!ui->plot->axisRect()->rect().contains(event->pos()))
    return;

  double x = colorMap->keyAxis()->pixelToCoord(event->pos().x());
  double y = colorMap->valueAxis()->pixelToCoord(event->pos().y());

  Options opts;
  opts.method = methodOptions->value(ui->method_comboBox->currentText());
  opts.function = functionOptions->value(ui->function_comboBox->currentText());
  opts.stop_criterion =
    stopCriterionOptions->value(ui->stopCriterion_comboBox->currentText());

  opts.initial_point = { x, y };
  opts.max_iterations = ui->maxIterations_spinBox->value();
  opts.eps = ui->eps_doubleSpinBox->value();
  opts.explore_probability = ui->p_doubleSpinBox->value();
  opts.delta = ui->delta_doubleSpinBox->value();
  opts.alpha = ui->alpha_doubleSpinBox->value();
  opts.initial_simplex_step = ui->simplexInitStep_doubleSpinBox->value();
  opts.xStart = ui->xStart_doubleSpinBox->value();
  opts.xEnd = ui->xEnd_doubleSpinBox->value();
  opts.yStart = ui->yStart_doubleSpinBox->value();
  opts.yEnd = ui->yEnd_doubleSpinBox->value();

  auto opt_result = run::run(opts);
  ui->plot->setCurrentLayer("points");
  ui->plot->graph(0)->data().data()->clear();
  for (auto& p : opt_result.point_history) {
    ui->plot->graph(0)->addData(p.first[0], p.first[1]);
  }

  std::stringstream ss;
  ss << "Result: (";
  ss << opt_result.point_history.back().first.transpose() << "), ";
  ss << "f = " << opt_result.point_history.back().second;
  ss << ", #iterations = " << opt_result.iteration_num;

  auto status_text = QString::fromStdString(ss.str());
  ui->statusbar->showMessage(status_text);

  ui->plot->replot();
}

void
MainWindow::on_stopCriterion_comboBox_currentIndexChanged(const QString& arg1)
{
  if (arg1.compare("Maximum iterations") == 0) {
    ui->eps_doubleSpinBox->setEnabled(false);
  } else {
    ui->eps_doubleSpinBox->setEnabled(true);
  }

  ui->statusbar->clearMessage();
  ui->plot->blockSignals(false);
}

void
MainWindow::on_method_comboBox_currentIndexChanged(const QString& arg1)
{
  ui->stopCriterion_comboBox->clear();
  ui->stopCriterion_comboBox->addItems(
    QStringList(stopCriterionOptions->keys()));
  ui->simplexInitStep_doubleSpinBox->show();
  ui->alpha_doubleSpinBox->show();
  ui->delta_doubleSpinBox->show();
  ui->p_doubleSpinBox->show();
  ui->label_6->show();
  ui->label_7->show();
  ui->label_8->show();
  ui->label_9->show();

  if (arg1.compare("Random search") == 0) {
    ui->stopCriterion_comboBox->removeItem(
      ui->stopCriterion_comboBox->findText("Minimum standard deviation"));
    ui->simplexInitStep_doubleSpinBox->hide();
    ui->label_6->hide();
  }

  if (arg1.compare("Nelder-Mead") == 0) {
    ui->alpha_doubleSpinBox->hide();
    ui->delta_doubleSpinBox->hide();
    ui->p_doubleSpinBox->hide();
    ui->label_7->hide();
    ui->label_8->hide();
    ui->label_9->hide();
  }
}
