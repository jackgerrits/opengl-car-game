#include "WaterShader.h"

WaterShader::WaterShader() : ShaderProgram(WATER_VERTEX_SHADER, WATER_FRAGMENT_SHADER) {
    bindUniformLocations();
}

void WaterShader::bindUniformLocations() {
    location_projection = glGetUniformLocation(shaderID, "projection");
    location_view = glGetUniformLocation(shaderID, "view");
    location_model = glGetUniformLocation(shaderID, "model");

    location_refractionTexture = glGetUniformLocation(shaderID, "refractionTexture");
    location_reflectionTexture = glGetUniformLocation(shaderID, "reflectionTexture");

    location_dudvMap = glGetUniformLocation(shaderID, "dudvMap");
    location_movement = glGetUniformLocation(shaderID, "movement");
    location_camera_position = glGetUniformLocation(shaderID, "camera_position");
    location_lightColour = glGetUniformLocation(shaderID, "lightColour");
    location_lightPosition = glGetUniformLocation(shaderID, "lightPosition");
}

void WaterShader::loadProjection(const glm::mat4& proj) {
    loadUniformValue(location_projection, proj);
}

void WaterShader::loadView(const glm::mat4& view) {
    loadUniformValue(location_view, view);
}

void WaterShader::loadModel(const glm::mat4& model) {
    loadUniformValue(location_model, model);
}

void WaterShader::loadMovement(float movement) {
    loadUniformValue(location_movement, movement);
}

void WaterShader::loadCameraPosition(const glm::vec3& position) {
    loadUniformValue(location_camera_position, position);
}

void WaterShader::loadLight(const Light* light) {
    loadUniformValue(location_lightColour, light->specular);
    loadUniformValue(location_lightPosition, glm::vec3(light->position));
}

void WaterShader::loadTextures() {
    loadUniformValue(location_refractionTexture, 0);
    loadUniformValue(location_reflectionTexture, 1);
    loadUniformValue(location_dudvMap, 2);
}