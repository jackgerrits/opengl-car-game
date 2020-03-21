#include "TerrainRenderer.h"

void TerrainRenderer::render(const Terrain* terrain, const std::vector<Light*>& lights, const glm::mat4& view,
    const glm::mat4& proj, const glm::vec4& clipPlane) {
    shader.enable();
    shader.loadProjection(proj);
    shader.loadLights(lights);
    shader.loadView(view);

    shader.loadTerrain(terrain);
    shader.loadClipPlane(clipPlane);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, terrain->getTextureID(0));
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, terrain->getTextureID(1));
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, terrain->getTextureID(2));
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, terrain->getTextureID(3));
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, terrain->getTextureID(4));

    glBindVertexArray(terrain->getVaoID());

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(terrain->getIndexCount()), GL_UNSIGNED_INT, (void*)0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glBindVertexArray(0);

    shader.disable();
}

void TerrainRenderer::render(const Terrain* terrain, const std::vector<Light*>& lights, const glm::mat4& view,
    const glm::mat4& proj, const glm::mat4& depthView, const glm::mat4& depthProj, GLuint shadowMap,
    const glm::vec4& clipPlane) {
    shader.enable();
    shader.loadProjection(proj);
    shader.loadLights(lights);
    shader.loadView(view);

    shader.loadTerrain(terrain);
    shader.loadClipPlane(clipPlane);
    // clang-format off
    glm::mat4 biasMatrix(
        0.5, 0.0, 0.0, 0.0,
        0.0, 0.5, 0.0, 0.0,
        0.0, 0.0, 0.5, 0.0,
        0.5, 0.5, 0.5, 1.0
    );
    // clang-format on
    glm::mat4 depthBiasPV = biasMatrix * depthProj * depthView;
    shader.loadDepth(depthBiasPV);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, terrain->getTextureID(0));
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, terrain->getTextureID(1));
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, terrain->getTextureID(2));
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, terrain->getTextureID(3));
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, terrain->getTextureID(4));
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, shadowMap);

    glBindVertexArray(terrain->getVaoID());

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(terrain->getIndexCount()), GL_UNSIGNED_INT, (void*)0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glBindVertexArray(0);

    shader.disable();
}
