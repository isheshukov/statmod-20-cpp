#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QString>
#include <cli.h>
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

private:
  Ui::MainWindow* ui;
  QMap<QString, Options::Method>* methodOptions;
  QMap<QString, Options::FunctionName>* functionOptions;
  QCPMarginGroup* marginGroup;
  QCPColorScale* colorScale;
  QCPColorMap* colorMap;
};

#endif // MAINWINDOW_H
