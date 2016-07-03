#include "ShadowMap.h"

ShadowMap::ShadowMap(Player* player, Light* light, GLuint textureSize) : 
    player(player), 
    fb(textureSize, textureSize),
    textureSize(textureSize) {

    projection = glm::ortho<float>(-30,30,-30,30,-50, 150);
    lightDir =  glm::vec3(light->position.x, light->position.y, light->position.z);

    fb.addDepthTexture();
    if(!fb.isOkay()){
        std::cerr << "[FrameBuffer] FrameBuffer is not okay." << std::endl;
        exit(1);
    }
}

GLuint ShadowMap::getTextureID(){
    return fb.getDepthTexture();
}

GLuint ShadowMap::getTextureSize(){
    return textureSize;
}

void ShadowMap::enableFramebuffer(){
    fb.bind();
    view = glm::lookAt(player->getPosition() + lightDir, player->getPosition(), glm::vec3(0,1,0));
}

void ShadowMap::disableFramebuffer(){
    fb.unbind();
}

glm::mat4 ShadowMap::getView(){
    return view;
}

glm::mat4 ShadowMap::getProjection(){
    return projection;
}
