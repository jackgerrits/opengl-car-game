#ifndef PARTICLE_RENDERER_H
#define PARTICLE_RENDERER_H

#define _USE_MATH_DEFINES

#include "ParticleShader.h"
#include "../entities/Camera.h"
#include "../Model.h"
#include "../Loader.h"

#include <cstdio>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include <glm/glm.hpp>
#pragma warning(push, 0)
#include <glm/ext.hpp>
#pragma warning(pop)
#include <GL/glew.h>

class ParticleRenderer {
private:
    ParticleShader shader;
    ModelComponent quad;
public:
    ParticleRenderer();

    void render(std::vector<Particle*> particles, glm::mat4 view, glm::mat4 proj);
};

#endif //PARTICLE_RENDERER_H
