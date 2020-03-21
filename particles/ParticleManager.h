#pragma once

#include "Particle.h"
#include "ParticleRenderer.h"
#include <glm/glm.hpp>
#include <vector>

class ParticleManager {
  private:
    static ParticleManager* particleManager;
    ParticleManager() = default;

    ParticleRenderer renderer;
    std::vector<Particle*> particles;

  public:
    static ParticleManager* getParticleManager();

    // Should be called once per frame
    void update();

    void addParticle(Particle*);
    void render(const glm::mat4& view, const glm::mat4& proj);
};
