#pragma once

#include "Entity.h"
#include "Terrain.h"
#include "../Model.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Player : public Entity {
private:
    const float MOVE_SPEED = 10.0f;
    float ROTATION_SPEED;

    // Extra parameters if physics mode is being used
    glm::vec2 velocity;
    glm::vec2 velocity_c;
    glm::vec2 accel;
    glm::vec2 accel_c;

    float yawRate;

    // Input parameters
    float steerAngle;
    float steerChange;
    float throttle_input;
    float brake_input;
    float ebrake_input;

    Terrain* terrain;

    // True to use basic controls, false to use physics model
    bool basic_controls;

    float smoothSteering(float);
public:
    Player(Model* model, Terrain* terrain, bool basic_controls);
    virtual bool update();
    float getThrottle();
    float getBrake();
    float getSteer();
    float absVel;

    void handleKeyboardEvents(GLFWwindow* window, int key, int scancode, int action, int mods);
};
