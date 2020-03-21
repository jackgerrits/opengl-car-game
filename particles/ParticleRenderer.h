#pragma once

#include "ParticleShader.h"
#include "../Model.h"

#include <vector>
#include <glm/glm.hpp>

class ParticleRenderer {
  private:
    ParticleShader shader;
    ModelComponent quad;

  public:
    ParticleRenderer();

    void render(std::vector<Particle*> particles, glm::mat4 view, glm::mat4 proj);
};
