#include "ParticleManager.h"

// Initialise singleton
ParticleManager* ParticleManager::particleManager = nullptr;

ParticleManager* ParticleManager::getParticleManager() {
    if (particleManager == nullptr) {
        particleManager = new ParticleManager();
    }

    return particleManager;
}

void ParticleManager::update() {
    // Update and kill particles
    for (auto particles_it = particles.begin(); particles_it != particles.end(); particles_it++) {
        if (!(*particles_it)->update()) {
            delete *particles_it;
            // TODO make particles a linked list so this erase is less costly.
            particles_it = particles.erase(particles_it);

            // If we just erased the last item in the vector we need to break out instead of iterating to the next
            // element.
            if (particles_it == particles.end()) {
                break;
            }
        }
    }
}

void ParticleManager::addParticle(Particle* particle) {
    particles.push_back(particle);
}

void ParticleManager::render(const glm::mat4& view, const glm::mat4& proj) {
    renderer.render(particles, view, proj);
}
