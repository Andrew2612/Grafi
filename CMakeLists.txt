cmake_minimum_required(VERSION 3.0.0)

project(Grafi VERSION 0.1.0 LANGUAGES C CXX)


add_library(EP ${PROJECT_SOURCE_DIR}/EdgeAndPoint/Edge_Point.cpp)
add_library(SC ${PROJECT_SOURCE_DIR}/Interface/ScreenController.cpp)
add_library(AS ${PROJECT_SOURCE_DIR}/ApplicationState/ApplicationState.cpp)


add_executable(Grafi main.cpp)

target_link_libraries(Grafi EP, SC, AS)
