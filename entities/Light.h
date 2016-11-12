#ifndef LIGHT_H
#define LIGHT_H

#include <cmath>

#include <GL/glew.h>
#include <glm/ext.hpp>

#define M_PI 3.14159265

// Lights are a simple struct holding all of the relevant information.
struct Light {
    Light(): 
        position(glm::vec4(0.0f)),
        specular(glm::vec3(0.0f)),
        diffuse(glm::vec3(0.0f)),
        ambient(glm::vec3(0.0f)),
        radius(0.0f),
        coneAngle((float)M_PI),
        coneDirection(glm::vec3(0.0f)){
    }

    glm::vec4 position; // Position of the camera in world coordinates. w value of 0 indicates directional light, 1 indicates positional/point light
    glm::vec3 specular; // Colour intensities of each light component
    glm::vec3 diffuse;
    glm::vec3 ambient;
    float radius;           // Radius used for attenuation
    float coneAngle;        // Angle and direction of cone for spotlights
    glm::vec3 coneDirection;
};

#endif
