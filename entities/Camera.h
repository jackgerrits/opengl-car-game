#ifndef CAMERA_H
#define CAMERA_H

#include "../InputState.h"
#include "Player.h"

#include <cmath>
#include <glm/glm.hpp>

const float WATER_PLANE_HEIGHT = 0.398918f;

/* Most of the camera code is reused from the example given */
class Camera {
protected:
    glm::mat4 viewMtx;
    glm::vec3 initEye;
    glm::vec3 initAt;
    glm::vec3 initUp;

public:
    Camera( glm::vec3 eye );

    const glm::mat4 getViewMtx() const;
    //    void orthogonaliseViewMtx();
    void reset();

    virtual void update( InputState &input ) = 0;
    virtual glm::mat4 getInverted() = 0;
    virtual glm::vec3 getPosition() = 0;
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

    virtual void update( InputState &input );
    virtual glm::mat4 getInverted();
    virtual glm::vec3 getPosition();
};


#endif