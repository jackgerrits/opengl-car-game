#include "SkyboxShader.h"

SkyboxShader::SkyboxShader(): ShaderProgram(SKYBOX_VERTEX_SHADER, SKYBOX_FRAGMENT_SHADER) {
    bindUniformLocations();
}

void SkyboxShader::bindUniformLocations(){
    location_projection = glGetUniformLocation(shaderID, "projection");
    location_view = glGetUniformLocation(shaderID, "view");
}

void SkyboxShader::loadMatrices(glm::mat4 camera, glm::mat4 projection){
    // Remove translation of camera matrix, just keep rotation
    camera[3][0] = 0.0f;
    camera[3][1] = 0.0f;
    camera[3][2] = 0.0f;

    loadUniformValue(location_view, camera);
    loadUniformValue(location_projection, projection);
}