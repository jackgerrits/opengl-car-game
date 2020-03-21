#pragma once

#include <glad/glad.h>
#include <cstdint>

class FrameBuffer {
  private:
    GLuint framebufferID;

    GLuint depthTexture;
    GLuint depthBuffer;
    GLuint colourTexture;

    GLuint width;
    GLuint height;

  public:
    FrameBuffer(uint32_t width, uint32_t height);
    void addColourTexture();
    void addDepthTexture();
    void addDepthBuffer();
    bool isOkay();

    GLuint getColourTexture();
    GLuint getDepthTexture();
    GLuint getDepthBuffer();

    int getWidth();
    int getHeight();

    virtual void bind();
    virtual void unbind();
};
