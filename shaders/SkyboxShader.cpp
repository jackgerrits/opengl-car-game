#include "SkyboxShader.h"

SkyboxShader::SkyboxShader() : ShaderProgram(SKYBOX_VERTEX_SHADER, SKYBOX_FRAGMENT_SHADER) {
    bindUniformLocations();
}

void SkyboxShader::bindUniformLocations() {
    location_projection = glGetUniformLocation(shaderID, "projection");
    location_view = glGetUniformLocation(shaderID, "view");
}

void SkyboxShader::loadMatrices(const glm::mat4& camera, const glm::mat4& projection) {
    auto camera_copy = camera;
    // Remove translation of camera matrix, just keep rotation
    camera_copy[3][0] = 0.0f;
    camera_copy[3][1] = 0.0f;
    camera_copy[3][2] = 0.0f;

    loadUniformValue(location_view, camera_copy);
    loadUniformValue(location_projection, projection);
}