cmake_minimum_required(VERSION 3.2.2)
project(gp_demo)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_executable(gp_demo main.cpp DemoApp.cpp systems/CameraMovementSystem.cpp systems/RotateSystem.cpp)

add_custom_command(TARGET gp_demo PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_CURRENT_SOURCE_DIR}/meshes $<TARGET_FILE_DIR:gp_demo>/meshes)
add_custom_command(TARGET gp_demo PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_CURRENT_SOURCE_DIR}/textures $<TARGET_FILE_DIR:gp_demo>/textures)

include_directories(${CMAKE_SOURCE_DIR}/graphics_playground)
target_link_libraries(gp_demo graphics_playground)

install(TARGETS gp_demo RUNTIME DESTINATION bin)
