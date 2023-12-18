###############################################################################
cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)
project(Portal_Maze)

###############################################################################

# Configure c++ version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_EXTENSIONS off)

# Create compile_commands.json for proper hints
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

###############################################################################

SOURCE_GROUP(sources REGULAR_EXPRESSION ".*/.*cpp$" )
SOURCE_GROUP(headers REGULAR_EXPRESSION ".*/.*hpp$" )
SOURCE_GROUP(shaders REGULAR_EXPRESSION ".*/.*(vert|frag|comp|geom|tese|tesc)$" )
SOURCE_GROUP(obj REGULAR_EXPRESSION ".*/.*obj$" )

###############################################################################

add_subdirectory(external)
set(EXTERNAL_LIBRARIES glew glfw glm SOIL tinyobjloader)

include_directories(src)

set(PROJECT_SOURCE_FILES
  src/main.cpp

  src/context.hpp
  src/context.cpp

  src/context.hpp
  src/mesh.cpp

  src/renderer.hpp
  src/renderer.cpp

  src/camera.hpp
  src/camera.cpp
  src/first_person_camera.cpp
  src/free_camera.cpp

  src/shader.hpp
  src/shader.cpp

  src/util.hpp
  src/util.cpp

  src/common.hpp
  src/orientable.cpp
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} ${EXTERNAL_LIBRARIES} ${INTERNAL_LIBRARIES})
