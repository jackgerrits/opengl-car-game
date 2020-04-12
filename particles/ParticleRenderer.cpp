#include "ParticleRenderer.h"

#include "../Loader.h"

#include <glad/glad.h>

ParticleRenderer::ParticleRenderer() {
    // clang-format off
    // Create a square mesh encompassing the viewport
    std::vector<float> vertices = {
        -0.5f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
    };

    std::vector<float> texcoords = {
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
    };

    std::vector<unsigned int> indices = {
        0, 1, 2,
        2, 3, 0
    };
    // clang-format on
    quad = Loader::getLoader()->loadModelComponent(vertices, indices, texcoords);
}

void ParticleRenderer::render(std::vector<Particle*> particles, glm::mat4 view, glm::mat4 proj) {
    shader.enable();

    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(quad.getVaoID());
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    shader.loadCamera(view);
    shader.loadProjection(proj);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    for (const auto& particle : particles) {
        shader.loadParticle(particle, view);
        glBindTexture(GL_TEXTURE_2D, particle->getTextureID());
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(quad.getIndexCount()), GL_UNSIGNED_INT, (void*)0);
    }

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindVertexArray(0);
    shader.disable();
}