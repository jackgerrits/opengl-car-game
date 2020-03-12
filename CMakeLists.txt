cmake_minimum_required(VERSION 3.13)

project(opengl-car-game LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(tiny_obj_loader OBJECT EXCLUDE_FROM_ALL)
target_sources(tiny_obj_loader PRIVATE libs/tiny_obj_loader.cpp libs/tiny_obj_loader.h)
target_include_directories(tiny_obj_loader PRIVATE ${CMAKE_CURRENT_LIST_DIR}/libs SYSTEM INTERFACE ${CMAKE_CURRENT_LIST_DIR}/libs)

add_library(stb_image OBJECT EXCLUDE_FROM_ALL)
target_sources(stb_image PRIVATE libs/stb_image.cpp libs/stb_image.h)
target_include_directories(stb_image PRIVATE ${CMAKE_CURRENT_LIST_DIR}/libs SYSTEM INTERFACE ${CMAKE_CURRENT_LIST_DIR}/libs)

if(MSVC)
  target_compile_options(stb_image PRIVATE /w)
  target_compile_options(tiny_obj_loader PRIVATE /w)
else(MSVC)
  target_compile_options(stb_image PRIVATE -w)
  target_compile_options(tiny_obj_loader PRIVATE -w)
endif(MSVC)

set(all_headers
  constants.h
  entities/Camera.h
  entities/Entity.h
  entities/Light.h
  entities/Player.h
  entities/Terrain.h
  FrameBuffer.h
  GameTime.h
  InputState.h
  Loader.h
  Model.h
  particles/Particle.h
  particles/ParticleManager.h
  particles/ParticleRenderer.h
  particles/ParticleShader.h
  particles/ParticleSystem.h
  renderers/EntityRenderer.h
  renderers/RenderManager.h
  renderers/SkyboxRenderer.h
  renderers/TerrainRenderer.h
  shaders/EntityShader.h
  shaders/ShaderProgram.h
  shaders/SkyboxShader.h
  shaders/TerrainShader.h
  ShadowMap.h
  water/WaterRenderer.h
  water/WaterShader.h
)

set(all_sources
  entities/Camera.cpp
  entities/Entity.cpp
  entities/Player.cpp
  entities/Terrain.cpp
  FrameBuffer.cpp
  GameTime.cpp
  Loader.cpp
  main.cpp
  Model.cpp
  particles/Particle.cpp
  particles/ParticleManager.cpp
  particles/ParticleRenderer.cpp
  particles/ParticleShader.cpp
  particles/ParticleSystem.cpp
  renderers/EntityRenderer.cpp
  renderers/RenderManager.cpp
  renderers/SkyboxRenderer.cpp
  renderers/TerrainRenderer.cpp
  shaders/EntityShader.cpp
  shaders/ShaderProgram.cpp
  shaders/SkyboxShader.cpp
  shaders/TerrainShader.cpp
  ShadowMap.cpp
  water/WaterRenderer.cpp
  water/WaterShader.cpp
)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(GLUT REQUIRED)

add_executable(opengl-car-game ${all_sources} ${all_headers})
target_link_libraries(opengl-car-game PRIVATE GLUT::GLUT OpenGL::GL glfw GLEW::GLEW glm stb_image tiny_obj_loader)

# Turn on more warnings
if(MSVC)
  target_compile_options(opengl-car-game PRIVATE /W4)
else(MSVC)
  target_compile_options(opengl-car-game PRIVATE -Wall -Wextra -Wpedantic)
endif(MSVC)

set_target_properties(opengl-car-game PROPERTIES VS_GLOBAL_RunCodeAnalysis "false")
set_target_properties(opengl-car-game PROPERTIES VS_GLOBAL_EnableClangTidyCodeAnalysis "true")
set_target_properties(opengl-car-game PROPERTIES VS_GLOBAL_ClangTidyChecks "-*,clang-analyzer-*,-clang-analyzer-cplusplus*,modernize-*,performance-*,readability-*,-modernize-use-trailing-return-type,-readability-uppercase-literal-suffix")

# Copy assets
add_custom_command(TARGET opengl-car-game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/res
    ${PROJECT_BINARY_DIR}/res
    COMMENT "Copy resources to build tree")

# Copy shaders
file(COPY ${CMAKE_SOURCE_DIR}/water/water.frag ${CMAKE_SOURCE_DIR}/water/water.vert
  DESTINATION ${PROJECT_BINARY_DIR}/water)
file(COPY ${CMAKE_SOURCE_DIR}/particles/particle.frag ${CMAKE_SOURCE_DIR}/particles/particle.vert
  DESTINATION ${PROJECT_BINARY_DIR}/particles)
file(COPY ${CMAKE_SOURCE_DIR}/shaders/entity.frag ${CMAKE_SOURCE_DIR}/shaders/entity.vert
  DESTINATION ${PROJECT_BINARY_DIR}/shaders)
file(COPY ${CMAKE_SOURCE_DIR}/shaders/skybox.frag ${CMAKE_SOURCE_DIR}/shaders/skybox.vert
  DESTINATION ${PROJECT_BINARY_DIR}/shaders)
file(COPY ${CMAKE_SOURCE_DIR}/shaders/terrain.frag ${CMAKE_SOURCE_DIR}/shaders/terrain.vert
  DESTINATION ${PROJECT_BINARY_DIR}/shaders)