# opengl-car-game

![](http://i.imgur.com/GGQdqUS.png)

Simple car game written in c++, using OpenGL 3.3.

Notable features include:
- Shadow mapping
- Water with dynamic reflection and refraction
- Dynamic particle system

### Dependencies

- OpenGL 3.3
- GLM
- GLFW
- GLEW

#### Install using Vcpkg on Windows
```
vcpkg install glfw3:x64-windows
vcpkg install glew:x64-windows
vcpkg install glm:x64-windows
vcpkg install freeglut:x64-windows
```

## Compilation

### CMake using Vcpkg
```
cmake .. -DCMAKE_TOOLCHAIN_FILE="[vcpkg root]\scripts\buildsystems\vcpkg.cmake" -DVCPKG_TARGET_TRIPLET=x64-windows
.\opengl-car-game.sln
```

### Make only
```
make
```

## Usage

```
USAGE: ./driver basic|physics
```
__basic__ - No intertia, simple turning

__physics__ - uses formula for more realistic driving experience.
