# opengl-car-game

![](http://i.imgur.com/GGQdqUS.png)

Simple car game written in C++, using OpenGL 3.3.

Notable features include:
- Shadow mapping
- Water with dynamic reflection and refraction
- Dynamic particle system

## Dependencies

### Ubuntu/Debian
```
sudo apt-get install freeglut3-dev libglfw3-dev libglew-dev libglm-dev
```

### Vcpkg on Windows
```
vcpkg install glfw3:x64-windows
vcpkg install glew:x64-windows
vcpkg install glm:x64-windows
vcpkg install freeglut:x64-windows
```

## Compilation

```
git lfs pull
mkdir build
cd build
cmake ..
make
```

### CMake using Vcpkg
```
git lfs pull
cmake .. -DCMAKE_TOOLCHAIN_FILE="[vcpkg root]\scripts\buildsystems\vcpkg.cmake" -DVCPKG_TARGET_TRIPLET=x64-windows
.\opengl-car-game.sln
```

## Usage

```
USAGE: ./driver basic|physics
```
__basic__ - No intertia, simple turning

__physics__ - uses formula for more realistic driving experience.
