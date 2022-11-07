project(graphics_playground)
cmake_minimum_required(VERSION 3.2.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE SOURCES RELATIVE "" "*.cpp")
list(FILTER SOURCES EXCLUDE REGEX "${PROJECT_SOURCE_DIR}/build/*")

find_package(glfw3 3.3 REQUIRED)
find_package(assimp REQUIRED)
find_package(glm REQUIRED)

add_library(graphics_playground ${SOURCES} "extern/glad/glad.c")

target_include_directories(graphics_playground PUBLIC "extern")
target_link_libraries(graphics_playground glfw)
target_link_libraries(graphics_playground assimp::assimp)
target_link_libraries(graphics_playground glm::glm)

install(TARGETS graphics_playground
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)