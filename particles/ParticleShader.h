#pragma once

#include "../entities/Camera.h"

#include "../shaders/ShaderProgram.h"
#include "Particle.h"

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glad/glad.h>

const std::string PARTICLE_VERTEX_SHADER = "particles/particle.vert";
const std::string PARTICLE_FRAGMENT_SHADER = "particles/particle.frag";

class ParticleShader : public ShaderProgram {
  private:
    GLuint location_projection;
    GLuint location_model;
    GLuint location_view;

  public:
    ParticleShader();

    void bindUniformLocations();
    void loadCamera(const glm::mat4& view);
    void loadParticle(const Particle *particle, const glm::mat4& viewmtx);
    void loadProjection(const glm::mat4& proj);
};
