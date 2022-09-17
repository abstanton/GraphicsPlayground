cmake_minimum_required(VERSION 3.2.2)
project(gp_demo)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_executable(gp_demo main.cpp DemoApp.cpp systems/CameraMovementSystem.cpp systems/RotateSystem.cpp)
include_directories(${CMAKE_SOURCE_DIR}/graphics_playground)
target_link_libraries(gp_demo graphics_playground)

install(TARGETS gp_demo RUNTIME DESTINATION bin)
