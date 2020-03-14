#ifndef PARTICLE_H
#define PARTICLE_H

#define _USE_MATH_DEFINES

#include "../Model.h"
#include "../entities/Entity.h"
#include "../Loader.h"
#include "../GameTime.h"

#include <string>
#include <iostream>
#include <GL/glew.h>
#pragma warning(push, 0)
#include <glm/ext.hpp>
#pragma warning(pop)
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