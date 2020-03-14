#include "EntityShader.h"

EntityShader::EntityShader() : ShaderProgram(ENTITY_VERTEX_SHADER, ENTITY_FRAGMENT_SHADER) {
    bindUniformLocations();
}

void EntityShader::bindUniformLocations() {
    // If named attributes are used the shader MUST be linked again after they are setup!
    // For now go back to using location based attributes
    // glBindAttribLocation(shaderID, 0, "a_vertex");
    // glBindAttribLocation(shaderID, 1, "a_normal");
    // glBindAttribLocation(shaderID, 2, "a_tex_coord");
    // glLinkProgram(this->shaderID);

    location_texMap = glGetUniformLocation(shaderID, "texMap");
    location_cubeMap = glGetUniformLocation(shaderID, "cubeMap");
    location_shadowMap = glGetUniformLocation(shaderID, "shadowMap");
    location_clip_plane = glGetUniformLocation(shaderID, "clip_plane");

    location_projection = glGetUniformLocation(shaderID, "projection");
    location_model = glGetUniformLocation(shaderID, "model");
    location_view = glGetUniformLocation(shaderID, "view");
    location_inv_view = glGetUniformLocation(shaderID, "inv_view");

    location_shininess = glGetUniformLocation(shaderID, "shininess");
    location_emission = glGetUniformLocation(shaderID, "emission");
    location_num_lights = glGetUniformLocation(shaderID, "num_lights");
    location_mtl_ambient = glGetUniformLocation(shaderID, "mtl_ambient");
    location_mtl_diffuse = glGetUniformLocation(shaderID, "mtl_diffuse");
    location_mtl_specular = glGetUniformLocation(shaderID, "mtl_specular");

    location_depth_pv = glGetUniformLocation(shaderID, "depth_pv");
    location_render_shadows = glGetUniformLocation(shaderID, "render_shadows");
}

void EntityShader::loadLights(const std::vector<Light*>& lights) {
    loadUniformValue(location_num_lights, static_cast<int>(lights.size()));
    for (size_t i = 0; i < lights.size(); i++) {
        loadLight(lights[i], i);
    }
}

void EntityShader::loadLight(Light* light, size_t i) {
    loadLightUniform("position", i, light->position);
    loadLightUniform("specular", i, light->specular);
    loadLightUniform("diffuse", i, light->diffuse);
    loadLightUniform("ambient", i, light->ambient);
    loadLightUniform("radius", i, light->radius);
    loadLightUniform("coneAngle", i, light->coneAngle);
    loadLightUniform("coneDirection", i, light->coneDirection);
}

void EntityShader::loadView(const glm::mat4& view) {
    loadUniformValue(location_view, view);
    loadUniformValue(location_inv_view, glm::inverse(view));
}

void EntityShader::loadEntity(Entity* entity) {
    loadUniformValue(location_texMap, 0);
    loadUniformValue(location_cubeMap, 1);
    glm::mat4 model = entity->calculateModelMatrix();

    loadUniformValue(location_model, model);
}

void EntityShader::loadModelComponent(const ModelComponent& component) {
    loadUniformValue(location_mtl_ambient, component.getMaterial().ambient, 3);
    loadUniformValue(location_mtl_diffuse, component.getMaterial().diffuse, 3);
    loadUniformValue(location_mtl_specular, component.getMaterial().specular, 3);
    loadUniformValue(location_emission, component.getMaterial().emission, 3);
    loadUniformValue(location_shininess, component.getMaterial().shininess);
}

void EntityShader::loadProjection(const glm::mat4& proj) {
    loadUniformValue(location_projection, proj);
    loadUniformValue(location_render_shadows, 0);
}
void EntityShader::loadClipPlane(const glm::vec4& clip) {
    loadUniformValue(location_clip_plane, clip);
}

void EntityShader::loadDepth(const glm::mat4& pv) {
    loadUniformValue(location_shadowMap, 2);
    loadUniformValue(location_depth_pv, pv);
    loadUniformValue(location_render_shadows, 1);
}