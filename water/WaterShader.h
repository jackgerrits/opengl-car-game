#pragma once

#include "../entities/Light.h"
#include "../shaders/ShaderProgram.h"

#include <string>

#include <glm/glm.hpp>
#include <glad/glad.h>

const std::string WATER_VERTEX_SHADER = "water/water.vert";
const std::string WATER_FRAGMENT_SHADER = "water/water.frag";

class WaterShader : public ShaderProgram {
  private:
    GLuint location_projection;
    GLuint location_view;
    GLuint location_model;

    GLuint location_refractionTexture;
    GLuint location_reflectionTexture;
    GLuint location_dudvMap;
    GLuint location_movement;
    GLuint location_camera_position;

    GLuint location_lightColour;
    GLuint location_lightPosition;

  public:
    WaterShader();

    void bindUniformLocations();

    void loadProjection(const glm::mat4& projection);
    void loadView(const glm::mat4& view);
    void loadModel(const glm::mat4& model);
    void loadMovement(float movement);
    void loadLight(const Light* light);
    void loadCameraPosition(const glm::vec3& position);
    void loadTextures();
};
