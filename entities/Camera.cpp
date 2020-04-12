#include "Camera.h"

#include <iostream>
#include <algorithm>
#include "../constants.h"
#include "../GameTime.h"

#include <glm/gtc/matrix_transform.hpp>

#define DEG2RAD(x) ((x)*constants::PI / 180.0)
#define RAD2DEG(x) ((x)*180.0 / constants::PI)

const glm::mat4& Camera::getViewMtx() const {
    return m_view_matrix;
}

const glm::vec3& Camera::getPosition() const {
    return m_position;
}
void Camera::setPosition(const glm::vec3& pos) {
    m_position = pos;
}

void Camera::look(const glm::vec3& at) {
    glm::vec3 up(0.0f, 1.0f, 0.0f);
    m_view_matrix = glm::lookAt(m_position, at, up);
}

void Camera::look(const glm::vec3& from, const glm::vec3& at) {
    glm::vec3 up(0.0f, 1.0f, 0.0f);
    m_view_matrix = glm::lookAt(from, at, up);
}

glm::mat4 Camera::getInverted(float pivotPoint) {
    glm::vec3 playerPos = m_focal_point;

    float offset = fabs(m_position.y - playerPos.y);
    glm::vec3 camPos = m_position - glm::vec3(0, 2 * offset, 0);

    camPos -= glm::vec3(0, playerPos.y - pivotPoint, 0);
    playerPos -= glm::vec3(0, playerPos.y - pivotPoint, 0);

    glm::vec3 up(0.0f, 1.0f, 0.0f);
    return glm::lookAt(camPos, playerPos, up);
}

PlayerCamera::PlayerCamera(Player* player) {
    m_player = player;
    m_distance = 5.0f;
    m_pitch = constants::PI / 8.f;
    m_angle_around = 0.0f;
}

void PlayerCamera::update(InputState& input) {
    m_distance -= input.scrollDeltaY;
    m_distance = std::clamp(m_distance, 2.5f, 20.0f);

    if (input.lMousePressed) {
        m_pitch -= input.deltaY / 50;
        m_pitch = std::clamp(m_pitch, 0.1f, constants::PI / 2 - 0.0001f);

        m_angle_around -= input.deltaX / 50.0f;

        if (m_angle_around > 2 * constants::PI) {
            m_angle_around -= 2 * constants::PI;
        }
        if (m_angle_around < -2 * constants::PI) {
            m_angle_around += 2 * constants::PI;
        }
    }
    input.scrollDeltaY = 0.0;
    input.deltaY = 0;
    input.deltaX = 0;

    float change = RESET_SPEED * GameTime::getGameTime()->getDt();

    if (m_player->getThrottle() > 0.1f) {
        if (fabs(m_angle_around) < change) {
            m_angle_around = 0.0f;
        } else if (m_angle_around > 0) {
            m_angle_around -= change;
        } else if (m_angle_around < 0) {
            m_angle_around += change;
        }
    }

    float angle = m_angle_around + m_player->getRotationY();

    float hDist = m_distance * glm::cos(m_pitch);
    float vDist = m_distance * glm::sin(m_pitch);
    float offsetX = hDist * glm::sin(angle);
    float offsetZ = hDist * glm::cos(angle);

    m_focal_point = m_player->getPosition();
    m_position = glm::vec3(-offsetX, vDist, -offsetZ) + m_focal_point;

    look(m_focal_point);
}
