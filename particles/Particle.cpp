#include "Particle.h"

#include "../GameTime.h"

#define GRAVITY_ACCELERATION -9.81f

Particle::Particle(
    const glm::vec3& position, const glm::vec3& velocity, float gravityEffect, float lifeDuration, GLuint texture) {
    this->m_position = position;
    this->velocity = velocity;
    this->gravityEffect = gravityEffect;
    this->lifeDuration = lifeDuration;
    this->elapsedTime = 0.0f;
    this->texid = texture;
}

// Returns true if the particle is still alive after this update.
bool Particle::update() {
    float dt = GameTime::getGameTime()->getDt();
    velocity.y += GRAVITY_ACCELERATION * gravityEffect * dt;
    m_position = m_position + (velocity * dt);
    elapsedTime += dt;
    return elapsedTime < lifeDuration;
}

GLuint Particle::getTextureID() const {
    return texid;
}
