#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "run.hpp"
#include <QMainWindow>
#include <QMap>
#include <QString>
#include <qcustomplot.h>

/*! \mainpage Optimization Methods
 *
 * \section intro_sec Introduction
 *
 * WIP
 *
 */

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

  void on_stopCriterion_comboBox_currentIndexChanged(const QString& arg1);

  void on_method_comboBox_currentIndexChanged(const QString& arg1);

private:
  Ui::MainWindow* ui;
  QMap<QString, Options::Method>* methodOptions;
  QMap<QString, Options::FunctionName>* functionOptions;
  QMap<QString, Options::StopCriterion>* stopCriterionOptions;

  QCPMarginGroup* marginGroup;
  QCPColorScale* colorScale;
  QCPColorMap* colorMap;
};

#endif // MAINWINDOW_H
