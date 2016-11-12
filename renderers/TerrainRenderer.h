#ifndef TERRAIN_RENDERER_H
#define TERRAIN_RENDERER_H

#define _USE_MATH_DEFINES

#include "../shaders/TerrainShader.h"
#include "../entities/Light.h"
#include "../entities/Camera.h"
#include "../Model.h"

#include <cstdio>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <GL/glew.h>

class TerrainRenderer {
private:
    TerrainShader shader;
public:
    TerrainRenderer();

    void render(Terrain* terrain, std::vector<Light*> lights, glm::mat4 view, glm::mat4 proj, glm::vec4 clipPlane);
    void render(Terrain* terrain, std::vector<Light*> lights, glm::mat4 view, glm::mat4 proj, glm::mat4 depthView, glm::mat4 depthProj, GLuint shadowMap, glm::vec4 clipPlane);
};

#endif //ENTITY_RENDERER_H

