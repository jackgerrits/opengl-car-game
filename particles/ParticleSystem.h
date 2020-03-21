#pragma once

#include "ParticleManager.h"

#include <glm/glm.hpp>
#include <glad/glad.h>

class ParticleSystem {
  private:
    float particlesPerSecond;
    float particleSpeed;
    float gravityFactor;
    float lifeDuration;
    GLuint textureid;
    float renderQueue;

  public:
    ParticleSystem(
        float particlesPerSecond, float particleSpeed, float gravityFactor, float lifeDuration, GLuint textureid);

    void generateParticles(glm::vec3 emissionPoint);
};
