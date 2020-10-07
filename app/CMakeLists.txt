cmake_minimum_required(VERSION 3.5)

project(nelder-mead LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

QT5_WRAP_UI(QT_UIS_HDRS
            mainwindow.ui)

add_executable(nelder-mead
  main.cpp
  mainwindow.cpp
  run.cpp
  ${QT_UIS_HDRS}
)
target_link_libraries(nelder-mead PUBLIC Qt5::Widgets optimization structopt mymath qcustomplot)
