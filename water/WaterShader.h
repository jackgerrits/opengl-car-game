#ifndef WATER_SHADER_H
#define WATER_SHADER_H

#define _USE_MATH_DEFINES

// #include "../entities/Entity.h"
#include "../entities/Light.h"
// #include "../entities/Camera.h"
#include "../Model.h"
#include "../shaders/ShaderProgram.h"

#include <cstdio>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <GL/glew.h>

const std::string WATER_VERTEX_SHADER = "water/water.vert";
const std::string WATER_FRAGMENT_SHADER = "water/water.frag";

class WaterShader : public ShaderProgram {
private:
    GLuint location_projection;
    GLuint location_view;
    GLuint location_model;

    GLuint location_refractionTexture;
    GLuint location_reflectionTexture;
    GLuint location_dudvMap;
    GLuint location_movement;
    GLuint location_camera_position;

    GLuint location_lightColour;
    GLuint location_lightPosition;
public:
    WaterShader();

    virtual void bindUniformLocations();

    void loadProjection(glm::mat4 projection);
    void loadView(glm::mat4 view);
    void loadModel(glm::mat4 model);
    void loadMovement(float movement);
    void loadLight(Light* light);
    void loadCameraPosition(glm::vec3 position);
    void loadTextures();
};

#endif //WATER_SHADER_H

