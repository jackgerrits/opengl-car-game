#ifndef PARTICLE_SHADER_H
#define PARTICLE_SHADER_H


#include "../entities/Camera.h"
#include "../Model.h"
#include "../shaders/ShaderProgram.h"
#include "Particle.h"

#include <cstdio>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <GL/glew.h>

const std::string PARTICLE_VERTEX_SHADER = "particles/particle.vert";
const std::string PARTICLE_FRAGMENT_SHADER = "particles/particle.frag";

class ParticleShader : public ShaderProgram {
private:
    GLuint location_projection;
    GLuint location_model;
    GLuint location_view;
public:
    ParticleShader();

    virtual void bindUniformLocations();
    void loadCamera(glm::mat4 view);
    void loadParticle(Particle* particle, glm::mat4 view);
    void loadProjection(glm::mat4 proj);
};

#endif //PARTICLE_SHADER_H

