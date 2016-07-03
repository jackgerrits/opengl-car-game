#include "WaterRenderer.h"

WaterRenderer::WaterRenderer(){
    this->shader = WaterShader();

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

    indexCount = indices.size();
    vao = Loader::getLoader()->loadVAO(vertices, indices);
    movement = 0.0f;

    dudvMap = Loader::getLoader()->loadTexture("res/water/dudv.png");
}

// Only works for one set of water, update to reflect
void WaterRenderer::render(Entity* water, glm::mat4 view, glm::mat4 projection, GLuint refract, GLuint reflect, glm::vec3 cameraPosition, Light* light){
    shader.enable();
    glDisable(GL_CULL_FACE);

    shader.loadProjection(projection);
    shader.loadView(view);
    shader.loadTextures();
    shader.loadMovement(movement);
    shader.loadCameraPosition(cameraPosition);
    shader.loadLight(light);

    movement += WATER_MOVE_SPEED * GameTime::getGameTime()->getDt();
    if(movement > 1.0f){
        movement -= 1.0f;
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, refract);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, reflect);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, dudvMap);

    shader.loadModel(water->getModelMatrix());
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, (void*)0);
    
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);

    glEnable(GL_CULL_FACE);
    shader.disable();
}