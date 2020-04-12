#include "ParticleShader.h"

ParticleShader::ParticleShader() : ShaderProgram(PARTICLE_VERTEX_SHADER, PARTICLE_FRAGMENT_SHADER) {
    bindUniformLocations();
}

void ParticleShader::bindUniformLocations() {
    location_projection = glGetUniformLocation(shaderID, "projection");
    location_model = glGetUniformLocation(shaderID, "model");
    location_view = glGetUniformLocation(shaderID, "view");
}

void ParticleShader::loadCamera(const glm::mat4& view) {
    loadUniformValue(location_view, view);
}

void ParticleShader::loadParticle(const Particle *particle, const glm::mat4& viewmtx) {
    auto modelmtx = particle->calculateModelMatrix();

    // Apply transpose of view matrix to make sure the particle always faces the camera.
    // TODO - test if glm::transpose can be used here
    modelmtx[0][0] = viewmtx[0][0];
    modelmtx[0][1] = viewmtx[1][0];
    modelmtx[0][2] = viewmtx[2][0];
    modelmtx[1][0] = viewmtx[0][1];
    modelmtx[1][1] = viewmtx[1][1];
    modelmtx[1][2] = viewmtx[2][1];
    modelmtx[2][0] = viewmtx[0][2];
    modelmtx[2][1] = viewmtx[1][2];
    modelmtx[2][2] = viewmtx[2][2];

    loadUniformValue(location_model, modelmtx);
}

void ParticleShader::loadProjection(const glm::mat4& proj) {
    loadUniformValue(location_projection, proj);
}
