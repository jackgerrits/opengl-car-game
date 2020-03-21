#pragma once

#include "../entities/Terrain.h"
#include "../entities/Light.h"
#include "ShaderProgram.h"

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glad/glad.h>

const std::string TERRAIN_VERTEX_SHADER = "shaders/terrain.vert";
const std::string TERRAIN_FRAGMENT_SHADER = "shaders/terrain.frag";

class TerrainShader : public ShaderProgram {
  private:
    GLuint location_blendMap;
    GLuint location_backMap;
    GLuint location_rMap;
    GLuint location_gMap;
    GLuint location_bMap;
    GLuint location_shadowMap;

    GLuint location_clip_plane;

    GLuint location_projection;
    GLuint location_model;
    GLuint location_view;

    GLuint location_depth_pv;

    GLuint location_num_lights;
    GLuint location_shininess;

  public:
    TerrainShader();

    void bindUniformLocations();

    void loadTerrain(const Terrain* terrain);

    void loadLights(const std::vector<Light*>& lights);
    void loadLight(Light* light, size_t i);
    void loadView(const glm::mat4& view);
    void loadDepth(const glm::mat4& pv);
    void loadClipPlane(const glm::vec4& clip);
    void loadProjection(const glm::mat4& proj);
};
