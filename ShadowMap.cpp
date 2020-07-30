#include "ShadowMap.h"

#include "glm_ext.h"

ShadowMap::ShadowMap(Player* player, Light* light, GLuint textureSize)
    : FrameBuffer(textureSize, textureSize), player(player), textureSize(textureSize) {
    projection = glm::ortho<float>(-30, 30, -30, 30, -50, 150);
    lightDir = glm::vec3(light->position.x, light->position.y, light->position.z);

    addDepthTexture();
    if (!isOkay()) {
        std::cerr << "[FrameBuffer] FrameBuffer is not okay." << std::endl;
        exit(1);
    }
}

GLuint ShadowMap::getTextureID() {
    return getDepthTexture();
}

GLuint ShadowMap::getTextureSize() {
    return textureSize;
}

void ShadowMap::bind() {
    FrameBuffer::bind();
    // Set the view matrix to be looking from sun's position to the player.
    view = glm::lookAt(player->getPosition() + lightDir, player->getPosition(), glm::vec3(0, 1, 0));
}

glm::mat4 ShadowMap::getView() {
    return view;
}

glm::mat4 ShadowMap::getProjection() {
    return projection;
}
