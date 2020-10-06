find_package(Qt5 COMPONENTS Core Widgets PrintSupport REQUIRED)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
  -Wno-deprecated-declarations")

add_library(qcustomplot qcustomplot.cpp)
target_include_directories(qcustomplot PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
target_link_libraries(qcustomplot Qt5::Widgets Qt5::PrintSupport)
