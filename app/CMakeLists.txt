cmake_minimum_required(VERSION 3.5)

project(nelder-mead LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5Core)

option(USE_GUI "Compile with GUI support" OFF)
if (USE_GUI)
  add_definitions(-DGUI)
endif()

add_executable(nelder-mead
  main.cpp
)
target_link_libraries(nelder-mead Qt5::Core optimization argh)
