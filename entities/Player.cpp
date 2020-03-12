#include "Player.h"

#include "../constants.h"

using namespace std;
using namespace glm;

/*
Car physics calculation has been modified from
    #1 https://github.com/spacejack/carphysics2d
which is an adaptation of
    http://www.asawicki.info/Mirror/Car%20Physics%20for%20Games/Car%20Physics%20for%20Games.html
I didnt make many changes since it is a complete equation, but I did add code to stop when there is no throttle.
*/

// Static variables used for car physics
float gravity = 9.81f;           // m/s^2
float mass = 2000.0f;            // kg
float inertiaScale = 1.0f;       // Multiply by mass for inertia
float halfWidth = 0.8f;          // Centre to side of chassis (metres)
float cgToFront = 1.5f;          // Centre of gravity to front of chassis (metres)
float cgToRear = 1.5f;           // Centre of gravity to rear of chassis
float cgToFrontAxle = 1.25f;     // Centre gravity to front axle
float cgToRearAxle = 1.25f;      // Centre gravity to rear axle
float cgHeight = 0.55f;          // Centre gravity height
float wheelRadius = 0.3f;        // Includes tire (also represents height of axle)
float wheelWidth = 0.2f;         // Used for render only
float tireGrip = 3.0f;           // How much grip tires have
float lockGrip = 0.8f;           // % of grip available when wheel is locked
float engineForce = 8000.0f;
float brakeForce = 12000.0f;
float eBrakeForce = brakeForce / 2.5f;
float weightTransfer = 0.2f;             // How much weight is transferred during acceleration/braking
float maxSteer = 0.6f;                   // Maximum steering angle in radians
float cornerStiffnessFront = 5.0f;
float cornerStiffnessRear = 5.2f;
float airResist = 3.0f;                    // air resistance (* vel)
float rollResist = 5.0f;
float inertia = mass * inertiaScale;    // will be = mass
float wheelBase = cgToFrontAxle + cgToRearAxle;         // set from axle to CG lengths
float axleWeightRatioFront = cgToRearAxle / wheelBase;  // % car weight on the front axle
float axleWeightRatioRear = cgToFrontAxle / wheelBase;  // % car weight on the rear axle


Player::Player(Model* model, Terrain* terrain, bool basic_controls): Entity(model){
    this->terrain = terrain;
    this->absVel = 0.0f;
    this->yawRate = 0.0f;
    this->steerAngle = 0.0f;
    this->steerChange = 0.0f;
    this->throttle_input = 0.0f;
    this->brake_input = 0.0f;
    this->ebrake_input = 0.0f;
    this->basic_controls = basic_controls;
    this->velocity = { 0.f, 0.f };
    this->velocity_c  = { 0.f, 0.f };
    this->accel = { 0.f, 0.f };
    this->accel_c = { 0.f, 0.f };
    if(basic_controls){
        ROTATION_SPEED = (float)constants::PI;
    } else {
        ROTATION_SPEED = 0.6f;
    }
}

template <typename T> int sgn(T val) {
return (T(0) < val) - (val < T(0));
}

float Player::getThrottle(){
    return throttle_input;
}

float Player::getBrake(){
    return brake_input;
}

float Player::getSteer(){
    return steerAngle;
}

bool Player::update(){
    steerAngle = smoothSteering(steerChange);
    float dt = GameTime::getGameTime()->getDt();
    float dx = 0.0f;
    float dz = 0.0f;

    if(basic_controls){
        rotateY(steerAngle * dt);

        if(yRot > (float)constants::PI*2){
            yRot -= constants::PI*2;
        } else if (yRot < (float)-constants::PI*2){
            yRot += constants::PI*2;
        }

        float distance = (throttle_input - brake_input) *  MOVE_SPEED * dt;

        dx = distance * glm::sin(yRot);
        dz = distance * glm::cos(yRot);
    } else {
        /* Following is code from #1 and adapted to this program */
        // Pre-calc heading vector
        float sn = sin(yRot);
        float cs = cos(yRot);

        // Get velocity in local car coordinates
        velocity_c.y = cs * velocity.y + sn * velocity.x;
        velocity_c.x = cs * velocity.x - sn * velocity.y;

        // Weight on axles based on centre of gravity and weight shift due to forward/reverse acceleration
        float axleWeightFront = mass * (axleWeightRatioFront * gravity - weightTransfer * accel_c.y * cgHeight / wheelBase);
        float axleWeightRear = mass * (axleWeightRatioRear * gravity + weightTransfer * accel_c.y * cgHeight / wheelBase);

        // Resulting velocity of the wheels as result of the yaw rate of the car body.
        // v = yawrate * r where r is distance from axle to CG and yawRate (angular velocity) in rad/s.
        float yawSpeedFront = cgToFrontAxle * yawRate;
        float yawSpeedRear = -cgToRearAxle * yawRate;

        // Calculate slip angles for front and rear wheels (a.k.a. alpha)
        float slipAngleFront = atan2(velocity_c.x + yawSpeedFront, abs(velocity_c.y)) - sgn(velocity_c.y) * steerAngle;
        float slipAngleRear  = atan2(velocity_c.x + yawSpeedRear,  abs(velocity_c.y));

        float tireGripFront = tireGrip;
        float tireGripRear = tireGrip * (1.0 - ebrake_input * (1.0 - lockGrip)); // reduce rear grip when ebrake is on

        float frictionForceFront_cy = std::clamp(-cornerStiffnessFront * slipAngleFront, -tireGripFront, tireGripFront) * axleWeightFront;
        float frictionForceRear_cy = std::clamp(-cornerStiffnessRear * slipAngleRear, -tireGripRear, tireGripRear) * axleWeightRear;

        //  Get amount of brake/throttle from our inputs
        float brake = std::min(brake_input * brakeForce + ebrake_input * eBrakeForce, brakeForce);
        float throttle = throttle_input * engineForce;

        //  Resulting force in local car coordinates.
        //  is implemented as a RWD car only.
        float tractionForce_cx = throttle - brake * sgn(velocity_c.y);
        float tractionForce_cy = 0;

        float dragForce_cx = -rollResist * velocity_c.y - airResist * velocity_c.y * abs(velocity_c.y);
        float dragForce_cy = -rollResist * velocity_c.x - airResist * velocity_c.x * abs(velocity_c.x);

        // total force in car coordinates
        float totalForce_cx = dragForce_cx + tractionForce_cx;
        float totalForce_cy = dragForce_cy + tractionForce_cy + cos(steerAngle) * frictionForceFront_cy + frictionForceRear_cy;

        // acceleration along car axes
        accel_c.y = totalForce_cx / mass;  // forward/reverse accel
        accel_c.x = totalForce_cy / mass;  // sideways accel

        // acceleration in world coordinates
        accel.y = cs * accel_c.y - sn * accel_c.x;
        accel.x = sn * accel_c.y + cs * accel_c.x;

        // update velocity
        velocity.y += accel.y * dt;
        velocity.x += accel.x * dt;

        absVel = length(velocity);

        // Slow the car down when no throttle, the overall equation doesn't seem to do this very well.
        if(throttle < 0.5f){
            velocity = velocity - (velocity * 0.5f * dt);
        }

        // calculate rotational forces
        float angularTorque = (frictionForceFront_cy + tractionForce_cy) * cgToFrontAxle - frictionForceRear_cy * cgToRearAxle;

        float angularAccel = angularTorque / inertia;

        //  Sim gets unstable at very slow speeds, so just stop the car
        if(abs(absVel) < 2.0f && throttle < 0.5f) {
            velocity.y = 0.0f;
            velocity.x = 0.0f;
            absVel = 0.0f;
            yawRate = 0.0f;
        } else {
            yawRate += angularAccel * dt;
            yRot += yawRate * dt;

            dx = velocity.x * dt;
            dz = velocity.y * dt;
        }
        /* End code from #1 */
    }

    // Wrap rotation around once it reaches 2*pi
    if(yRot > (float)constants::PI*2){
        yRot -= constants::PI*2;
    } else if (yRot < (float)-constants::PI*2){
        yRot += constants::PI*2;
    }

    // Assumes constant scale
    float player_length = (abs(model->getRangeInDim(2).second - model->getRangeInDim(2).first))/2.0f * scale.x;
    float player_length_x = player_length * glm::sin(yRot);
    float player_length_z = player_length * glm::cos(yRot);

    // Currently, acceleration and velocity are maintained on collision with edge.
    // TODO zero velocity and acceleration on FIRST collision in incident, so as to allow escaping the wall
    if(terrain->isOnTerrain(position.x + dx + player_length_x, position.z + dz + player_length_z)){
        move(glm::vec3(dx, 0, dz));
        placeBottomEdge(terrain->getHeight(getPosition().x, getPosition().z));
        setRotationX(terrain->getAngleX(getPosition().x, getPosition().z, getRotationY()));
        setRotationZ(terrain->getAngleZ(getPosition().x, getPosition().z, getRotationY()));
        return true;
    }
    return false;
}

float Player::smoothSteering(float inputAngle){
    float smoothedAngle = 0;
    float dt = GameTime::getGameTime()->getDt();
    float CHANGE_MODIFIER = 12.0f;

    if(abs(inputAngle) > 0.001 ){
        smoothedAngle = std::clamp((float)(steerAngle + inputAngle * dt * CHANGE_MODIFIER), -ROTATION_SPEED, ROTATION_SPEED);
    } else {
        //  No steer input - move toward centre (0)
        if( steerAngle > 0 ) {
            smoothedAngle = std::max(steerAngle - dt * CHANGE_MODIFIER, 0.0f);
        }
        else if( steerAngle < 0 ){
            smoothedAngle = std::min(steerAngle + dt * CHANGE_MODIFIER, 0.0f);
        }
    }

    return smoothedAngle;
}

void Player::handleKeyboardEvents(GLFWwindow* /*window*/, int key, int /*scancode*/, int action, int /*mods*/){
    if(action == GLFW_PRESS){
        if(key == GLFW_KEY_W || key == GLFW_KEY_UP){
            throttle_input = 1.0f;
        }
        if(key == GLFW_KEY_S || key == GLFW_KEY_DOWN){
            brake_input = 1.0f;
        }
        if(key == GLFW_KEY_SPACE){
            ebrake_input = 1.0f;
        }
        if(key == GLFW_KEY_A || key == GLFW_KEY_LEFT){
            steerChange = ROTATION_SPEED;
        } else if(key == GLFW_KEY_D || key == GLFW_KEY_RIGHT){
            steerChange = -ROTATION_SPEED;
        }
    }

    if(action == GLFW_RELEASE){
        if(key == GLFW_KEY_W || key == GLFW_KEY_UP){
            throttle_input = 0.0f;
        }
        if(key == GLFW_KEY_S || key == GLFW_KEY_DOWN){
            brake_input = 0.0f;
        }
        if(key == GLFW_KEY_SPACE){
            ebrake_input = 0.0f;
        }

        if((key == GLFW_KEY_A || key == GLFW_KEY_LEFT) && steerAngle > 0.0f){
           steerChange = 0.0f;
       }

       if((key == GLFW_KEY_D || key == GLFW_KEY_RIGHT) && steerAngle < 0.0f){
           steerChange = 0.0f;
       }
   }
}