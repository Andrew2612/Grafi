cmake_minimum_required(VERSION 3.0.0)
project(Grafi VERSION 0.1.0)

include(CTest)
enable_testing()

set(CMAKE_PREFIX_PATH ${PROJECT_SOURCE_DIR}/../SFML-2.6.1)

set(SFML_STATIC_LIBRARIES TRUE)

find_package(SFML COMPONENTS graphics REQUIRED)
find_package(SFML COMPONENTS window REQUIRED)
find_package(SFML COMPONENTS system REQUIRED)

add_executable(Grafi main.cpp)

target_link_libraries(Grafi sfml-graphics)


add_library(EP ${PROJECT_SOURCE_DIR}/../EdgeAndPoint/Edge_Point.cpp)
target_link_libraries(EP sfml-graphics)
target_link_libraries(Grafi EP)

add_library(IH ${PROJECT_SOURCE_DIR}/../Interface/InputHandler.cpp)
target_link_libraries(IH EP)
target_link_libraries(IH sfml-graphics)
target_link_libraries(Grafi IH)

add_library(SC ${PROJECT_SOURCE_DIR}/../Interface/ScreenController.cpp)
target_link_libraries(SC sfml-graphics)
target_link_libraries(Grafi SC)

add_library(M ${PROJECT_SOURCE_DIR}/../Map/Map.cpp)
target_link_libraries(M EP)
target_link_libraries(M sfml-graphics)
target_link_libraries(Grafi M)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
