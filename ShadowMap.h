#pragma once

#include "entities/Light.h"
#include "entities/Player.h"
#include "FrameBuffer.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

class ShadowMap : public FrameBuffer {
  private:
    Player* player;

    const GLuint textureSize;

    glm::vec3 lightDir;
    glm::mat4 projection;
    glm::mat4 view;

  public:
    ShadowMap(Player* player, Light* light, GLuint textureSize = 2048);

    GLuint getTextureID();
    GLuint getTextureSize();
    glm::mat4 getView();
    glm::mat4 getProjection();

    // Override default bind of FrameBuffer to update state.
    // Unbind is unchanged
    void bind() override;
};
