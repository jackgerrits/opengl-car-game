#pragma once

#define _USE_MATH_DEFINES

#include "Particle.h"
#include "../entities/Camera.h"
#include "ParticleRenderer.h"
#include <GLFW/glfw3.h>
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
