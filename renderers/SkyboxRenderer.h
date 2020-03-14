#pragma once

#include "../shaders/SkyboxShader.h"

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

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
    SkyboxRenderer(const std::vector<std::string>& images, const float SIZE);
    GLuint getSkyboxTexture() const;
    void render(const glm::mat4& view, const glm::mat4&);
};
