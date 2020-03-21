#include "EntityRenderer.h"

void EntityRenderer::render(const std::vector<Entity*>& entities, const std::vector<Light*>& lights,
    const glm::mat4& view, const glm::mat4& proj, GLuint reflectionTexture, const glm::vec4& clipPlane) {
    m_shader.enable();
    m_shader.loadProjection(proj);
    m_shader.loadLights(lights);
    m_shader.loadView(view);
    m_shader.loadClipPlane(clipPlane);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, reflectionTexture);

    for (const auto& entity : entities) {
        m_shader.loadEntity(entity);
        if (entity->getModel() != nullptr) {
            renderModel(entity->getModel());
        }
    }

    m_shader.disable();
}

void EntityRenderer::render(const std::vector<Entity*>& entities, const std::vector<Light*>& lights,
    const glm::mat4& view, const glm::mat4& proj, GLuint reflectionTexture, const glm::mat4& depthView,
    const glm::mat4& depthProj, GLuint shadowMap, const glm::vec4& clipPlane) {
    m_shader.enable();
    m_shader.loadProjection(proj);
    m_shader.loadLights(lights);
    m_shader.loadView(view);
    m_shader.loadClipPlane(clipPlane);

    glm::mat4 biasMatrix(0.5, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.5, 0.5, 0.5, 1.0);

    glm::mat4 depthBiasPV = biasMatrix * depthProj * depthView;
    m_shader.loadDepth(depthBiasPV);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, reflectionTexture);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, shadowMap);

    for (const auto& entity : entities) {
        m_shader.loadEntity(entity);
        if (entity->getModel() != nullptr) {
            renderModel(entity->getModel());
        }
    }

    m_shader.disable();
}

void EntityRenderer::renderModel(const Model* model) {
    for (const auto& component : model->getModelComponents()) {
        m_shader.loadModelComponent(component);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, component.getTextureID());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glBindVertexArray(component.getVaoID());

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(component.getIndexCount()), GL_UNSIGNED_INT, (void*)0);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        glBindVertexArray(0);
    }
}