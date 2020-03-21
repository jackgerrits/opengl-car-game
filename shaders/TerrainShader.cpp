#include "TerrainShader.h"

TerrainShader::TerrainShader() : ShaderProgram(TERRAIN_VERTEX_SHADER, TERRAIN_FRAGMENT_SHADER) {
    bindUniformLocations();
}

void TerrainShader::bindUniformLocations() {
    location_blendMap = glGetUniformLocation(shaderID, "blendMap");
    location_backMap = glGetUniformLocation(shaderID, "backMap");
    location_rMap = glGetUniformLocation(shaderID, "rMap");
    location_gMap = glGetUniformLocation(shaderID, "gMap");
    location_bMap = glGetUniformLocation(shaderID, "bMap");

    location_clip_plane = glGetUniformLocation(shaderID, "clip_plane");

    location_projection = glGetUniformLocation(shaderID, "projection");
    location_model = glGetUniformLocation(shaderID, "model");
    location_view = glGetUniformLocation(shaderID, "view");
    location_depth_pv = glGetUniformLocation(shaderID, "depth_pv");
    location_shadowMap = glGetUniformLocation(shaderID, "shadowMap");

    location_shininess = glGetUniformLocation(shaderID, "shininess");
    location_num_lights = glGetUniformLocation(shaderID, "num_lights");
}

void TerrainShader::loadLights(const std::vector<Light*>& lights) {
    loadUniformValue(location_num_lights, int(lights.size()));
    for (size_t i = 0; i < lights.size(); i++) {
        loadLight(lights[i], i);
    }
}

void TerrainShader::loadLight(Light* light, size_t i) {
    loadLightUniform("position", i, light->position);
    loadLightUniform("specular", i, light->specular);
    loadLightUniform("diffuse", i, light->diffuse);
    loadLightUniform("ambient", i, light->ambient);
    loadLightUniform("radius", i, light->radius);
    loadLightUniform("coneAngle", i, light->coneAngle);
    loadLightUniform("coneDirection", i, light->coneDirection);
}

void TerrainShader::loadView(const glm::mat4& view) {
    loadUniformValue(location_view, view);
}

void TerrainShader::loadDepth(const glm::mat4& pv) {
    loadUniformValue(location_depth_pv, pv);
    loadUniformValue(location_shadowMap, 5);
}

void TerrainShader::loadTerrain(const Terrain* terrain) {
    loadUniformValue(location_blendMap, 0);
    loadUniformValue(location_backMap, 1);
    loadUniformValue(location_rMap, 2);
    loadUniformValue(location_gMap, 3);
    loadUniformValue(location_bMap, 4);

    glm::mat4 model = terrain->calculateModelMatrix();
    loadUniformValue(location_model, model);
}

void TerrainShader::loadClipPlane(const glm::vec4& clip) {
    loadUniformValue(location_clip_plane, clip);
}

void TerrainShader::loadProjection(const glm::mat4& proj) {
    loadUniformValue(location_projection, proj);
}