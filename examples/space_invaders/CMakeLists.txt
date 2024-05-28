cmake_minimum_required(VERSION 3.2.0)
project(space_invaders)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_executable(space_invaders main.cpp SpaceInvaders.cpp systems/PlayerControllerSystem.cpp systems/BulletSystem.cpp systems/AsteroidSystem.cpp)

add_custom_command(TARGET space_invaders PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_CURRENT_SOURCE_DIR}/meshes $<TARGET_FILE_DIR:space_invaders>/meshes)
add_custom_command(TARGET space_invaders PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_CURRENT_SOURCE_DIR}/textures $<TARGET_FILE_DIR:space_invaders>/textures)

include_directories(${CMAKE_SOURCE_DIR}/graphics_playground)
target_link_libraries(space_invaders graphics_playground)

install(TARGETS space_invaders RUNTIME DESTINATION bin)
