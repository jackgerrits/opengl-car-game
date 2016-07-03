#include "ParticleManager.h"

// Initialise singleton
ParticleManager* ParticleManager::particleManager = NULL;

ParticleManager::ParticleManager(){

}

ParticleManager* ParticleManager::getParticleManager(){
    if(particleManager == NULL){
        particleManager = new ParticleManager();
    }

    return particleManager;
}

void ParticleManager::update(){
     // Update and kill particles
    for(int i = particles.size() - 1; i >= 0 ; i--){
        if(!particles[i]->update()){
            delete particles[i];
            particles.erase(particles.begin() + i);
        }
    }
}

void ParticleManager::addParticle(Particle* particle){
    particles.push_back(particle);
}

void ParticleManager::render(glm::mat4 view, glm::mat4 proj){
    renderer.render(particles, view, proj);
}
