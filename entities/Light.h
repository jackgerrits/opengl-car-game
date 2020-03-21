#pragma once

#include "../constants.h"
#include <glm/glm.hpp>

// Lights are a simple struct holding all of the relevant information.
struct Light {
    // Position of the camera in world coordinates. w value of 0 indicates directional light, 1 indicates
    // positional/point light
    glm::vec4 position = glm::vec4(0.0f);
    // Colour intensities of each light component
    glm::vec3 specular = glm::vec3(0.0f);
    glm::vec3 diffuse = glm::vec3(0.0f);
    glm::vec3 ambient = glm::vec3(0.0f);
    // Radius used for attenuation
    float radius = 0.f;
    // Angle and direction of cone for spotlights
    float coneAngle = constants::PI;
    glm::vec3 coneDirection = glm::vec3(0.0f);
};
