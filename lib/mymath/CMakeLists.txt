find_package(Eigen3 REQUIRED NO_MODULE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(mymath STATIC mymath.cpp)
target_include_directories(mymath PUBLIC include)
target_link_libraries(mymath Eigen3::Eigen)
