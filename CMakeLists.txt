###############################################################################
cmake_minimum_required(VERSION 3.5.0 FATAL_ERROR)
project(graphics_project)

###############################################################################

# Configure c++ version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_EXTENSIONS off)

# Create compile_commands.json for proper hints
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

find_package(OpenGL REQUIRED)

###############################################################################

SOURCE_GROUP(common REGULAR_EXPRESSION ".*/common/.*" )
SOURCE_GROUP(shaders REGULAR_EXPRESSION ".*/.*(vert|frag|comp|geom|tese|tesc)$" )
SOURCE_GROUP(obj REGULAR_EXPRESSION ".*/.*obj$" )

###############################################################################

add_subdirectory(external)

include_directories(
  src
  )

add_executable(${PROJECT_NAME}
  src/main.cpp
  src/mesh.h
  src/mesh.cpp
  src/common/shader.cpp
  src/common/util.cpp
  src/context.cpp
  src/context.hpp
  )

target_link_libraries(${PROJECT_NAME}
  ${OPENGL_LIBRARY}
  glfw
  glew
  glm
  )

