#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "run.hpp"
#include <QMainWindow>
#include <QMap>
#include <QString>
#include <qcustomplot.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

private slots:
  void on_actionExit_triggered();

  void on_function_comboBox_currentIndexChanged(QString value);
  void on_plot_click(QMouseEvent* event);

private:
  Ui::MainWindow* ui;
  QMap<QString, Options::Method>* methodOptions;
  QMap<QString, Options::FunctionName>* functionOptions;
  QCPMarginGroup* marginGroup;
  QCPColorScale* colorScale;
  QCPColorMap* colorMap;
};

#endif // MAINWINDOW_H
