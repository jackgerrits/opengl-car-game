#pragma once

#include "../entities/Entity.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

class Particle : public Entity {
  protected:
    glm::vec3 velocity;
    float gravityEffect;
    float lifeDuration;
    float elapsedTime;
    GLuint texid;

  public:
    Particle(
        const glm::vec3& position, const glm::vec3& velocity, float gravityEffect, float lifeDuration, GLuint texture);
    bool update() override;
    GLuint getTextureID() const;
};
