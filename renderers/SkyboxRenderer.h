#pragma once

#include "../shaders/SkyboxShader.h"

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>

class SkyboxRenderer {
  private:
    SkyboxShader shader;
    GLuint vao;
    GLuint texture;
    size_t indexCount;

  public:
    /*
    Images should be in order:
        PosXFilename,
        NegXFilename,
        PosYFilename,
        NegYFilename,
        PosZFilename,
        NegZFilename
    */
    SkyboxRenderer(const std::vector<std::string>& images, float SIZE);
    GLuint getSkyboxTexture() const;
    void render(const glm::mat4& view, const glm::mat4&);
};
