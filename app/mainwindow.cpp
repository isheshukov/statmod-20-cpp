#include "mainwindow.h"
#include "run.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  methodOptions = new QMap<QString, Options::Method>;
  functionOptions = new QMap<QString, Options::FunctionName>;

  methodOptions->insert("Random search", Options::Method::random);
  methodOptions->insert("Nelder-Mead", Options::Method::nelder_mead);

  functionOptions->insert("Sphere", Options::FunctionName::sphere);
  functionOptions->insert("Easom", Options::FunctionName::easom);
  functionOptions->insert("Rastrigin", Options::FunctionName::rastrigin);
  functionOptions->insert("Rosenbrock", Options::FunctionName::rosenbrock);

  ui->plot->axisRect()->setupFullAxesBox(true);
  ui->plot->xAxis->setLabel("x");
  ui->plot->yAxis->setLabel("y");

  colorMap = new QCPColorMap(ui->plot->xAxis, ui->plot->yAxis);
  colorScale = new QCPColorScale(ui->plot);
  marginGroup = new QCPMarginGroup(ui->plot);
  int nx = 400;
  int ny = 400;
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
  // ui->method_comboBox->setCurrentIndex(0);

  ui->function_comboBox->addItems(QStringList(functionOptions->keys()));
  // ui->function_comboBox->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
  delete ui;
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

  int nx = 400;
  int ny = 400;

  double x, y, z;
  for (int xIndex = 0; xIndex < nx; ++xIndex) {
    for (int yIndex = 0; yIndex < ny; ++yIndex) {
      colorMap->data()->cellToCoord(xIndex, yIndex, &x, &y);
      double r = functions.at(functionOptions->value(value))(Vector2d(x, y));
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
  double x = colorMap->keyAxis()->pixelToCoord(event->pos().x());
  double y = colorMap->valueAxis()->pixelToCoord(event->pos().y());

  Options opts;
  opts.stop_criterion = Options::StopCriterion::num_iterations;
  opts.method = methodOptions->value(ui->method_comboBox->currentText());
  opts.function = functionOptions->value(ui->function_comboBox->currentText());
  opts.initial_point = { x, y };

  std::cout << "x = " << opts.initial_point[0]
            << "y = " << opts.initial_point[1] << std::endl;

  auto opt_result = run::run(opts);

  ui->plot->setCurrentLayer("points");
  ui->plot->graph(0)->data().data()->clear();
  for (auto& p : opt_result.point_history) {
    // std::cout << "x = " << p.first[0] << "y = " << p.first[1] << std::endl;
    ui->plot->graph(0)->addData(p.first[0], p.first[1]);
  }

  std::cout << opt_result << std::endl;

  ui->plot->replot();
}
