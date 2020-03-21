#pragma once

#include "ShaderProgram.h"

#include <string>
#include <glm/glm.hpp>
#include <glad/glad.h>

const std::string SKYBOX_VERTEX_SHADER = "shaders/skybox.vert";
const std::string SKYBOX_FRAGMENT_SHADER = "shaders/skybox.frag";

class SkyboxShader : public ShaderProgram {
  private:
    GLuint location_projection;
    GLuint location_view;

  public:
    SkyboxShader();

    void bindUniformLocations();
    void loadMatrices(const glm::mat4& camera, const glm::mat4& projection);
};
