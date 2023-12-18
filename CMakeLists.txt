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

file(GLOB_RECURSE sources src/*.cpp)
file(GLOB_RECURSE headers src/*.hpp)
file(GLOB_RECURSE shaders shaders/*)
file(GLOB_RECURSE models resources/models/*)
file(GLOB_RECURSE textures resources/textures/*)

###############################################################################

add_subdirectory(external)
set(EXTERNAL_LIBRARIES glew glfw glm SOIL tinyobjloader)

include_directories(src)

add_executable(${PROJECT_NAME} ${sources} ${headers})

target_link_libraries(${PROJECT_NAME} ${EXTERNAL_LIBRARIES})

set (resources_src "${CMAKE_SOURCE_DIR}/resources")
set (resources_dst "${CMAKE_CURRENT_BINARY_DIR}/resources")

add_custom_command(
 TARGET ${PROJECT_NAME} POST_BUILD
 COMMAND ${CMAKE_COMMAND} -E create_symlink ${resources_src} ${resources_dst}
 DEPENDS ${resources_dst}
 COMMENT "Symbolic link resources folder"
)

set (shaders_src "${CMAKE_SOURCE_DIR}/shaders")
set (shaders_dst "${CMAKE_CURRENT_BINARY_DIR}/shaders")

add_custom_command(
 TARGET ${PROJECT_NAME} POST_BUILD
 COMMAND ${CMAKE_COMMAND} -E create_symlink ${shaders_src} ${shaders_dst}
 DEPENDS ${shaders_dst}
 COMMENT "symbolic link shaders folder"
)
