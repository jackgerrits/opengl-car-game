#ifndef CAMERA_H
#define CAMERA_H

#define _USE_MATH_DEFINES
#include <cmath>

#include "../InputState.h"
#include "Player.h"

#include <glm/glm.hpp>

class Camera {
protected:
    glm::vec3 position;
    glm::vec3 focalPoint;

    glm::mat4 viewMtx;
public:
    Camera();

    const glm::mat4 getViewMtx() const;

    glm::vec3 getPosition();
    void setPosition(glm::vec3 position);
    glm::mat4 getInverted(float pivotPoint);

    virtual void update(InputState &input);

    void look(glm::vec3 at);
    void look(glm::vec3 from, glm::vec3 at);
};


class PlayerCamera : public Camera {
private:
    Player* player;
    float distance;
    float pitch;
    float angleAround;

    float RESET_SPEED = (float)M_PI/2;
public:
    PlayerCamera(Player* player);

    virtual void update(InputState &input);
};

#endif
