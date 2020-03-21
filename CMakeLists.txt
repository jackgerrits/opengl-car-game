cmake_minimum_required(VERSION 3.14)

project(opengl-car-game LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(extern EXCLUDE_FROM_ALL)

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

add_executable(opengl-car-game ${all_sources} ${all_headers})
target_link_libraries(opengl-car-game PRIVATE glad::glad OpenGL::GL glfw glm stb_image tiny_obj_loader ${CMAKE_DL_LIBS})

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
    ${CMAKE_CURRENT_LIST_DIR}/res/
    ${PROJECT_BINARY_DIR}/res/
    COMMENT "Copy resources to build tree")

function(target_add_shader target out_dir)
  foreach(source_shader_file ${ARGN})
    get_filename_component(shader_file_name ${source_shader_file} NAME)
    set(out_shader_file "${out_dir}${shader_file_name}")
    file(MAKE_DIRECTORY ${out_dir})
    file(TOUCH ${out_shader_file})
    file(CREATE_LINK ${source_shader_file} ${out_shader_file})
  endforeach()
endfunction()

# Copy shaders
target_add_shader(opengl-car-game ${PROJECT_BINARY_DIR}/water/
  ${CMAKE_CURRENT_LIST_DIR}/water/water.frag
  ${CMAKE_CURRENT_LIST_DIR}/water/water.vert
)

target_add_shader(opengl-car-game ${PROJECT_BINARY_DIR}/particles/
  ${CMAKE_CURRENT_LIST_DIR}/particles/particle.frag
  ${CMAKE_CURRENT_LIST_DIR}/particles/particle.vert
)

target_add_shader(opengl-car-game ${PROJECT_BINARY_DIR}/shaders/
  ${CMAKE_CURRENT_LIST_DIR}/shaders/entity.frag
  ${CMAKE_CURRENT_LIST_DIR}/shaders/entity.vert
  ${CMAKE_CURRENT_LIST_DIR}/shaders/skybox.frag
  ${CMAKE_CURRENT_LIST_DIR}/shaders/skybox.vert
  ${CMAKE_CURRENT_LIST_DIR}/shaders/terrain.frag
  ${CMAKE_CURRENT_LIST_DIR}/shaders/terrain.vert
)
