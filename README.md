# Portal Maze

Project for the Graphics & Virtual Reality subject of the ECE course from University of Patras

### Clone repository

HTTPS
```bash
git clone --recurse-submodules https://github.com/alk-alyss/graphics
```

SSH
```bash
git clone --recurse-submodules git@github.com:alk-alyss/graphics
```

### Dependencies

- [GLEW](https://github.com/nigels-com/glew)>=2.2.0 - The OpenGL Extension Wrangler Library
- [GLFW](https://github.com/glfw/glfw)>=3.3 - A multi-platform library for OpenGL, OpenGL ES, Vulkan, window and input
- [GLM](https://github.com/g-truc/glm)>=0.9.9 - OpenGL Mathematics (GLM) is a header only C++ mathematics library for graphics software based on the OpenGL Shading Language (GLSL) specifications
- [SOIL2](https://github.com/SpartanJ/SOIL2)>=1.0 - Simple OpenGL Image Library
- [tinyobjloader](https://github.com/tinyobjloader/tinyobjloader)>=2.0 - Tiny but powerful single file wavefront obj loader

All the dependencies are included in the repository as submodules.
To compile the project you only need a C++ compiler capable of compiling C++17 programs and CMake >= 3.16

```bash

### Build instructions

```bash
git submodule init
git submodule update
cmake -B build
cmake --build build
```

### Run project

```bash
./build/Portal_Maze
```

or

```bash
cd build
./Portal_Maze
```
