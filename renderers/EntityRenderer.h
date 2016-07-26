#ifndef ENTITY_RENDERER_H
#define ENTITY_RENDERER_H

#include "../shaders/EntityShader.h"
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

class EntityRenderer {
private:
    EntityShader shader;
public:
    EntityRenderer();

    void render(std::vector<Entity*> entities, std::vector<Light*> lights, glm::mat4 view, glm::mat4 proj,  GLuint reflectionTexture, glm::vec4 clipPlane);
    void render(std::vector<Entity*> entities, std::vector<Light*> lights, glm::mat4 view, glm::mat4 proj,  GLuint reflectionTexture, glm::mat4 depthView, glm::mat4 depthProj, GLuint shadowMap, glm::vec4 clipPlane);
    void renderModel(Model* model);
};

#endif //ENTITY_RENDERER_H

