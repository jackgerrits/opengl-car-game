#ifndef PARTICLE_H
#define PARTICLE_H

#include "../Model.h"
#include "../entities/Entity.h"
#include "../Loader.h"
#include "../GameTime.h"

#include <assert.h>
#include <string>
#include <iostream>
#include <GL/glew.h>
#include <glm/ext.hpp>
#include <glm/glm.hpp>

#define GRAVITY_ACCELERATION -9.81f

class Particle : public Entity {
protected:
    glm::vec3 velocity;
    float gravityEffect;
    float lifeDuration;
    float elapsedTime;
    GLuint texid;

public:
    Particle(glm::vec3 position, glm::vec3 velocity, float gravityEffect, float lifeDuration, GLuint texture);
    bool update();
    GLuint getTextureID();
};




#endif