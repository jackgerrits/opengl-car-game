#include "Camera.h"

#include <stdio.h>
#include <iostream>
#include <algorithm>
#include "../constants.h"

#include <glm/gtc/matrix_transform.hpp>

#define DEG2RAD(x) ((x)*constants::PI/180.0)
#define RAD2DEG(x) ((x)*180.0/constants::PI)

Camera::Camera()
    : position(glm::vec3(0.0f))
    , focalPoint(glm::vec3(0.0f))
    , viewMtx(glm::mat4(1.0f))
{ }

const glm::mat4 Camera::getViewMtx() const {
    return viewMtx;
}

glm::vec3 Camera::getPosition(){
    return position;
}
void Camera::setPosition(glm::vec3 pos){
    this->position = pos;
}
void Camera::update(InputState& /*input*/){
    // Input has no effect in the base object.
}

void Camera::look(glm::vec3 at){
    glm::vec3 up(0.0f, 1.0f, 0.0f);
    viewMtx = glm::lookAt(position, at, up);
}

void Camera::look(glm::vec3 from, glm::vec3 at){
    glm::vec3 up(0.0f, 1.0f, 0.0f);
    viewMtx = glm::lookAt(from, at, up);
}


glm::mat4 Camera::getInverted(float WATER_HEIGHT){
    glm::vec3 playerPos = this->focalPoint;

    float offset = fabs(this->position.y - playerPos.y);
    glm::vec3 camPos = this->position - glm::vec3(0, 2*offset, 0);

    camPos -= glm::vec3(0, playerPos.y - WATER_HEIGHT, 0);
    playerPos -= glm::vec3(0, playerPos.y - WATER_HEIGHT, 0);

    glm::vec3 up(0.0f, 1.0f, 0.0f);
    return glm::lookAt(camPos, playerPos, up);
}

PlayerCamera::PlayerCamera(Player* player) : Camera(){
    this->player = player;
    this->distance = 5.0f;
    this->pitch = (float)constants::PI/8;
    this->angleAround = 0.0f;
}

void PlayerCamera::update(InputState &input){
    distance -= input.scrollDeltaY;
    distance = std::clamp(distance, 2.5f, 20.0f);

    if(input.lMousePressed){
        pitch -= input.deltaY / 50;
        pitch = std::clamp(pitch, 0.1f, constants::PI/2 - 0.0001f);

        angleAround -= input.deltaX / 50.0f;

        if(angleAround > 2*constants::PI){
            angleAround -= 2*constants::PI;
        }
        if(angleAround < -2*constants::PI){
            angleAround += 2*constants::PI;
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

    this->focalPoint = player->getPosition();
    this->position = glm::vec3(-offsetX, vDist, -offsetZ) + this->focalPoint;

    look(this->focalPoint);
}
