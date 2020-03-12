#include "Particle.h"

using namespace std;

Particle::Particle(glm::vec3 position, glm::vec3 velocity, float gravityEffect, float lifeDuration, GLuint texture) {
    this->position = position;
    this->velocity = velocity;
    this->gravityEffect = gravityEffect;
    this->lifeDuration = lifeDuration;
    this->elapsedTime = 0.0f;
    this->texid = texture;

}

// true - particle is alive
bool Particle::update(){
    float dt = GameTime::getGameTime()->getDt();
    velocity.y += GRAVITY_ACCELERATION * gravityEffect * dt;
    position = position + (velocity * dt);
    elapsedTime += dt;
    return elapsedTime < lifeDuration;
}

GLuint Particle::getTextureID(){
    return texid;
}