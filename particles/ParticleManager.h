#ifndef PARTICLE_MANAGER_H
#define PARTICLE_MANAGER_H

#define _USE_MATH_DEFINES

#include "Particle.h"
#include "../entities/Camera.h"
#include "ParticleRenderer.h"
#include <GLFW/glfw3.h>
#include <vector>

class ParticleManager {
private:
    static ParticleManager* particleManager;
    ParticleManager();
    
    ParticleRenderer renderer;
    std::vector<Particle*> particles;

public:
    static ParticleManager* getParticleManager();

    void update();  // Should be called once per frame

	void addParticle(Particle*);
	void render(glm::mat4 view, glm::mat4 proj);

};

#endif
