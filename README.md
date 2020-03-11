# opengl-car-game

![](http://i.imgur.com/GGQdqUS.png)

Simple car game written in C++, using OpenGL 3.3.

Notable features include:
- Shadow mapping
- Water with dynamic reflection and refraction
- Dynamic particle system

## Dependencies

##### Ubuntu/Debian
```
sudo apt-get install freeglut3-dev libglfw3-dev libglew-dev libglm-dev
```

##### Vcpkg on Windows
```
vcpkg install --triplet x64-windows glfw3 glew glm freeglut
```

## Build

If dependencies are visible to CMake by default, can build like so:
```
git lfs pull
mkdir build
cd build
cmake ..
make
```

Otherwise, you'll need to point CMake to the Vcpkg toolchain file to find the dependencies and specify the triplet. For example on Windows:

```
git lfs pull
cmake .. -DCMAKE_TOOLCHAIN_FILE="[vcpkg root]\scripts\buildsystems\vcpkg.cmake" -DVCPKG_TARGET_TRIPLET=x64-windows
.\opengl-car-game.sln
```

## Usage

```
./opengl-car-game basic|physics
```
__basic__ - No intertia, simple turning

__physics__ - uses formula for more realistic driving experience.
