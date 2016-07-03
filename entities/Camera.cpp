#include "Camera.h"

#include <stdio.h>
#include <iostream>
#include <math.h>
#include <algorithm>

template <typename T>
T clamp(const T& n, const T& lower, const T& upper) {
  return std::max(lower, std::min(n, upper));
}

#include "glm/gtc/matrix_transform.hpp"

#define DEG2RAD(x) ((x)*M_PI/180.0) 
#define RAD2DEG(x) ((x)*180.0/M_PI) 

Camera::Camera( glm::vec3 eye ) {
    initEye = eye;
    reset();
}

const glm::mat4 Camera::getViewMtx() const {
    return viewMtx;
}

/**
 Resets the view matrix to the value the camera was 
 initialised with. Assumes looking at the origin.
*/
void Camera::reset() {
    glm::vec3 at(0.0f, 0.0f, 0.0f);
    glm::vec3 up(0.0f, 1.0f, 0.0f);
    viewMtx = glm::lookAt(initEye, at, up);
}

PlayerCamera::PlayerCamera(Player* player) : Camera(glm::vec3(0.0f,0.0f,0.0f)){
    this->player = player;
    this->distance = 5.0f;
    this->pitch = (float)M_PI/8;
    this->angleAround = 0.0f;
}

void PlayerCamera::update(InputState &input){
    distance -= input.scrollDeltaY;
    distance = clamp(distance, 2.5f, 20.0f);

    if(input.lMousePressed){
        pitch -= input.deltaY / 50;
        pitch = clamp(pitch, 0.1f, (float)M_PI/2 - 0.0001f);

        angleAround -= input.deltaX / 50.0f;

        if(angleAround > (float)2*M_PI){
            angleAround -= (float)2*M_PI;
        }
        if(angleAround < (float)-2*M_PI){
            angleAround += (float)2*M_PI;
        }
    }
    input.scrollDeltaY = 0.0;
    input.deltaY = 0;
    input.deltaX = 0;

    float change = RESET_SPEED * GameTime::getGameTime()->getDt();

    if(player->getThrottle() > 0.1f){
        if(fabs(angleAround) < change){
            angleAround = 0.0f;
        }else if(angleAround > 0){
            angleAround -= change;
        } else if (angleAround < 0){
            angleAround += change;
        }
    }

    float angle = angleAround + player->getRotationY();

    float hDist = distance * glm::cos(pitch);
    float vDist = distance * glm::sin(pitch);
    float offsetX = hDist * glm::sin(angle);
    float offsetZ = hDist * glm::cos(angle);

    glm::vec3 camPos(-offsetX, vDist, -offsetZ);
    camPos = camPos + player->getPosition();

    glm::vec3 up(0.0f, 1.0f, 0.0f);
    viewMtx = glm::lookAt(camPos, player->getPosition(), up);
}

glm::mat4 PlayerCamera::getInverted(){
    float angle = angleAround + player->getRotationY();

    float hDist = distance * glm::cos(pitch);
    float vDist = distance * glm::sin(pitch);
    float offsetX = hDist * glm::sin(angle);
    float offsetZ = hDist * glm::cos(angle);

    glm::vec3 camPos(-offsetX, -vDist, -offsetZ);
    // 0.4 is midpoint of car to base. Cannot seem to get the reflections to work unless the center of the car is aligned with the water plane.
    // Surely this can be fixed based on the fact that it works if their centres align.
    camPos = camPos + glm::vec3(player->getPosition().x, WATER_PLANE_HEIGHT, player->getPosition().z);

    glm::vec3 up(0.0f, 1.0f, 0.0f);
    return glm::lookAt(camPos,glm::vec3(player->getPosition().x, WATER_PLANE_HEIGHT, player->getPosition().z), up);
}

glm::vec3 PlayerCamera::getPosition(){
    float angle = angleAround + player->getRotationY();

    float hDist = distance * glm::cos(pitch);
    float vDist = distance * glm::sin(pitch);
    float offsetX = hDist * glm::sin(angle);
    float offsetZ = hDist * glm::cos(angle);

    glm::vec3 camPos(-offsetX, vDist, -offsetZ);
    return camPos + glm::vec3(player->getPosition().x, WATER_PLANE_HEIGHT, player->getPosition().z);
}