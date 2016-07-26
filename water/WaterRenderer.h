#ifndef WATER_RENDERER_H
#define WATER_RENDERER_H

#include "WaterShader.h"
#include "../entities/Entity.h"
#include "../Loader.h"
#include "../GameTime.h"

#include <string>
#include <vector>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <GL/glew.h>

/*
https://www.youtube.com/playlist?list=PLRIWtICgwaX23jiqVByUs0bqhnalNTNZh

This tutorial was used to implement water. 
Therefore, there are similarities to the code presented there, however I did try and implement it with my own spin and differences.
But especially in the shaders it isn't THAT different, so I am referencing because it is the work of another, but it only makes up a small bit of the overall project.
*/

const float WATER_MOVE_SPEED = 0.04f;

class WaterRenderer {
private:
    WaterShader shader;
    GLuint vao;
    int indexCount;

    GLuint dudvMap;

    float movement;
public:
    WaterRenderer();

    void render(Entity* water, glm::mat4 view, glm::mat4 projection, GLuint refract, GLuint reflect, glm::vec3 cameraPosition, Light* light);
};

#endif //WATER_RENDERER_H

