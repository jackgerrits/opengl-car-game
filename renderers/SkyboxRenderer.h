#ifndef SKYBOX_RENDERER_H
#define SKYBOX_RENDERER_H

#include "../shaders/SkyboxShader.h"
#include "../entities/Light.h"
#include "../entities/Camera.h"
#include "../Model.h"
#include "../Loader.h"

#include <cstdio>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <GL/glew.h>

class SkyboxRenderer {
private:
    SkyboxShader shader;
    GLuint vao;
    GLuint texture;
    int indexCount;
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
    SkyboxRenderer(std::vector<std::string> images, const float SIZE);
    GLuint getSkyboxTexture();
    void render(glm::mat4 view, glm::mat4);
};

#endif //SKYBOX_RENDERER_H

