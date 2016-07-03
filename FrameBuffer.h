#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include "entities/Light.h"
#include "entities/Player.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <cfloat>
#include <utility>
#include <algorithm>
#include <iostream>

class FrameBuffer {
private:
    GLuint framebufferID;

    GLuint depthTexture;
    GLuint depthBuffer;
    GLuint colourTexture;

    GLuint width;
    GLuint height;
public:
    FrameBuffer(int width, int height);
    void addColourTexture();
    void addDepthTexture();
    void addDepthBuffer();
    bool isOkay();

    GLuint getColourTexture();
    GLuint getDepthTexture();
    GLuint getDepthBuffer();

    int getWidth();
    int getHeight();

    void bind();
    void unbind();
};

#endif