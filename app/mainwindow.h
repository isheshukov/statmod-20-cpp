#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <qcustomplot.h>
#include <QMainWindow>
#include <QMap>
#include <QString>
#include "run.hpp"

/*! \mainpage Optimization Methods

\image html ./screen.png

\section Compilation
\code{.sh}
git clone --recurse-submodules https://github.com/isheshukov/statmod-20-cpp.git
mkdir statmod-20-cpp/build
cd statmod-20-cpp/build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
./app/nelder-mead
\endcode

\section Usage

\code{.sh}
USAGE: optimization_methods [FLAGS] [OPTIONS] initial_point

FLAGS:
    -g, --gui  Start in GUI mode

OPTIONS:
    -m, --method <method> ... nelder_mead, random
    -f, --function <function> ... sphere, easom, rosenbrock, rastrigin,
himmelblau

    -s, --stop-criterion <stop_criterion> ... min_std_dev, num_iterations
    -m, --max-iterations <max_iterations> ... for min_std_dev and num_iterations
     -e, --eps <eps> ... for min_std_dev

 NELDER-MEAD OPTIONS:
     -i, --initial-simplex_step <initial_simplex_step>

 RANDOM SEARCH OPTIONS:
     -e, --explore-probability <explore_probability>
     -d, --delta <delta>
     -a, --alpha <alpha>

 SEARCH SPACE OPTIONS (2D only):
     -x, --xStart <xStart>
     -x, --xEnd <xEnd>
     -y, --yStart <yStart>
     -y, --yEnd <yEnd>

     -h, --help <help>
     -v, --version <version>

 ARGS:
     initial_point ... Initial point of optimization

 \endcode
*/

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
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

#endif  // MAINWINDOW_H
