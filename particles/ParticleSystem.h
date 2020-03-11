#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#define _USE_MATH_DEFINES

#include "ParticleManager.h"
#include "../entities/Camera.h"
#include "../Model.h"
#include "../Loader.h"

#include <cstdio>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>


#include <glm/glm.hpp>
#pragma warning(push, 0)
#include <glm/ext.hpp>
#pragma warning(pop)
#include <GL/glew.h>

class ParticleSystem {
private:
    float particlesPerSecond;
    float particleSpeed;
    float gravityFactor;
    float lifeDuration;
    GLuint textureid;

    float renderQueue;

public:
    ParticleSystem(float particlesPerSecond, float particleSpeed, float gravityFactor, float lifeDuration, GLuint textureid);

    void generateParticles(glm::vec3 emissionPoint);
};

#endif //PARTICLE_SYSTEM_H