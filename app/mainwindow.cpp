#include "mainwindow.h"
#include "cli.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  QMap<QString, Options::Method>* methodOptions =
    new QMap<QString, Options::Method>;

  methodOptions->insert("Random search", Options::Method::random);
  methodOptions->insert("Nelder-Mead", Options::Method::nelder_mead);

  ui->method_comboBox->addItems(QStringList(methodOptions->keys()));
  ui->method_comboBox->setCurrentIndex(1);

  auto customPlot = ui->plot;
  // configure axis rect:
  // customPlot->setInteractions(
  //  QCP::iRangeDrag | QCP::iRangeZoom); // this will also allow rescaling the
  // color scale by dragging/zooming
  customPlot->axisRect()->setupFullAxesBox(true);
  customPlot->xAxis->setLabel("x");
  customPlot->yAxis->setLabel("y");

  // set up the QCPColorMap:
  QCPColorMap* colorMap = new QCPColorMap(customPlot->xAxis, customPlot->yAxis);
  int nx = 400;
  int ny = 400;
  colorMap->data()->setSize(
    nx, ny); // we want the color map to have nx * ny data points
  colorMap->data()->setRange(
    QCPRange(-10, 10),
    QCPRange(-10, 10)); // and span the coordinate range -4..4 in
                        // both key (x) and value (y) dimensions
  // now we assign some data, by accessing the QCPColorMapData instance of the
  // color map:
  double x, y, z;
  for (int xIndex = 0; xIndex < nx; ++xIndex) {
    for (int yIndex = 0; yIndex < ny; ++yIndex) {
      colorMap->data()->cellToCoord(xIndex, yIndex, &x, &y);
      double r = functions.at(Options::FunctionName::sphere)(Vector2d(x, y));
      colorMap->data()->setCell(xIndex, yIndex, r);
    }
  }

  // add a color scale:
  QCPColorScale* colorScale = new QCPColorScale(customPlot);
  customPlot->plotLayout()->addElement(
    0, 1, colorScale); // add it to the right of the main axis rect
  colorScale->setType(
    QCPAxis::atRight); // scale shall be vertical bar with tick/axis labels
                       // right (actually atRight is already the default)
  colorMap->setColorScale(
    colorScale); // associate the color map with the color scale
  // colorScale->axis()->setLabel("Magnetic Field Strength");

  // set the color gradient of the color map to one of the presets:
  colorMap->setGradient(QCPColorGradient::gpPolar);
  // we could have also created a QCPColorGradient instance and added own colors
  // to the gradient, see the documentation of QCPColorGradient for what's
  // possible.

  // rescale the data dimension (color) such that all data points lie in the
  // span visualized by the color gradient:
  colorMap->rescaleDataRange();

  // make sure the axis rect and color scale synchronize their bottom and top
  // margins (so they line up):
  QCPMarginGroup* marginGroup = new QCPMarginGroup(customPlot);
  customPlot->axisRect()->setMarginGroup(QCP::msBottom | QCP::msTop,
                                         marginGroup);
  colorScale->setMarginGroup(QCP::msBottom | QCP::msTop, marginGroup);

  // rescale the key (x) and value (y) axes so the whole color map is visible:
  customPlot->rescaleAxes();
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
