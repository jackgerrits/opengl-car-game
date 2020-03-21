#include "WaterRenderer.h"

#include "../Loader.h"
#include "../GameTime.h"

WaterRenderer::WaterRenderer() {
    // clang-format off
    std::vector<float> vertices = {
        -1.0f, 0.0f, 1.0f,
        -1.0f, 0.0f, -1.0f,
        1.0f, 0.0f, -1.0f,
        1.0f, 0.0f, 1.0f,
    };

    std::vector<unsigned int> indices = {
        0, 3, 2,
        2, 1, 0
    };
    // clang-format on
    indexCount = indices.size();
    vao = Loader::getLoader()->loadVAO(vertices, indices);
    movement = 0.0f;

    dudvMap = Loader::getLoader()->loadTexture("res/water/dudv.png");
}

// Only works for one set of water, update to reflect
void WaterRenderer::render(const Entity* water, const glm::mat4& view, const glm::mat4& projection, GLuint refract,
    GLuint reflect, const glm::vec3& cameraPosition, const Light* light) {
    shader.enable();
    glDisable(GL_CULL_FACE);

    shader.loadProjection(projection);
    shader.loadView(view);
    shader.loadTextures();
    shader.loadMovement(movement);
    shader.loadCameraPosition(cameraPosition);
    shader.loadLight(light);

    movement += WATER_MOVE_SPEED * GameTime::getGameTime()->getDt();
    if (movement > 1.0f) {
        movement -= 1.0f;
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, refract);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, reflect);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, dudvMap);

    shader.loadModel(water->calculateModelMatrix());
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);

    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount), GL_UNSIGNED_INT, (void*)0);

    glDisableVertexAttribArray(0);
    glBindVertexArray(0);

    glEnable(GL_CULL_FACE);
    shader.disable();
}