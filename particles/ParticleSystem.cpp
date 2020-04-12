#include "ParticleSystem.h"

#include "Particle.h"
#include "ParticleManager.h"
#include "../GameTime.h"

ParticleSystem::ParticleSystem(
    float particlesPerSecond, float particleSpeed, float gravityFactor, float lifeDuration, GLuint textureid) {
    this->particlesPerSecond = particlesPerSecond;
    this->particleSpeed = particleSpeed;
    this->gravityFactor = gravityFactor;
    this->lifeDuration = lifeDuration;
    this->textureid = textureid;
    this->renderQueue = 0.0f;
}

void ParticleSystem::generateParticles(glm::vec3 emissionPoint) {
    renderQueue += particlesPerSecond * GameTime::getGameTime()->getDt();
    while (renderQueue > 1.0f) {
        glm::vec3 velo((rand() % 100) / 50.f - 1.0f, 1.0f, (rand() % 100) / 50.f - 1.0f);
        velo = glm::normalize(velo);
        velo *= particleSpeed;
        auto* p = new Particle(emissionPoint, velo, gravityFactor, lifeDuration, textureid);
        float scaleVal = (rand() % 100) / 100.f;
        p->setScale(glm::vec3(scaleVal, scaleVal, scaleVal));

        ParticleManager::getParticleManager()->addParticle(p);
        renderQueue -= 1.0f;
    }
}