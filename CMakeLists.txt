cmake_minimum_required(VERSION 3.8)

project(opengl-car-game LANGUAGES CXX)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(all_headers
  entities/Camera.h
  entities/Entity.h
  entities/Light.h
  entities/Player.h
  entities/Terrain.h
  FrameBuffer.h
  GameTime.h
  InputState.h
  libs/stb_image.h
  libs/tiny_obj_loader.h
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
  libs/stb_image.cpp
  libs/tiny_obj_loader.cpp
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
find_package(GLM REQUIRED)
find_package(GLUT REQUIRED)

add_executable(opengl-car-game ${all_sources} ${all_headers})
target_link_libraries(opengl-car-game PRIVATE GLUT::GLUT OpenGL::GL glfw GLEW::GLEW)
# Am I able to do
# target_link_libraries(opengl-car-game PRIVATE glm)
target_include_directories(opengl-car-game PRIVATE ${GLM_INCLUDE_DIRS})

add_custom_command(TARGET opengl-car-game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/res
    ${PROJECT_BINARY_DIR}/res
    COMMENT "Copy resources to build tree")

# TODO: This is overkill, scope it down later.
add_custom_command(TARGET opengl-car-game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/shaders
    ${PROJECT_BINARY_DIR}/shaders
    COMMENT "Copy shaders to build tree")
add_custom_command(TARGET opengl-car-game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/particles
    ${PROJECT_BINARY_DIR}/particles
    COMMENT "Copy shaders to build tree")
add_custom_command(TARGET opengl-car-game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/water
    ${PROJECT_BINARY_DIR}/water
    COMMENT "Copy shaders to build tree")