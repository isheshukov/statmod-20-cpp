set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_library(optimization STATIC optimization.cpp stop_criterion.cpp optimization_parameters.cpp method.cpp)
target_include_directories(optimization PUBLIC include)
target_link_libraries(optimization mymath)
