#include "EntityRenderer.h"

EntityRenderer::EntityRenderer(){
    this->shader = EntityShader();
}

void EntityRenderer::render(std::vector<Entity*> entities, std::vector<Light*> lights, glm::mat4 view, glm::mat4 proj, GLuint reflectionTexture, glm::vec4 clipPlane){
    shader.enable();
    shader.loadProjection(proj);
    shader.loadLights(lights);
    shader.loadView(view);
    shader.loadClipPlane(clipPlane);


    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, reflectionTexture);

    for(size_t i = 0; i < entities.size(); ++i){
        shader.loadEntity(entities[i]);
        if(entities[i]->getModel() != NULL){
            renderModel(entities[i]->getModel());
        } 
    }

    shader.disable();
}

void EntityRenderer::render(std::vector<Entity*> entities, std::vector<Light*> lights, glm::mat4 view, glm::mat4 proj,  GLuint reflectionTexture, glm::mat4 depthView, glm::mat4 depthProj, GLuint shadowMap, glm::vec4 clipPlane){
    shader.enable();
    shader.loadProjection(proj);
    shader.loadLights(lights);
    shader.loadView(view);
    shader.loadClipPlane(clipPlane);

    glm::mat4 biasMatrix(
        0.5, 0.0, 0.0, 0.0,
        0.0, 0.5, 0.0, 0.0,
        0.0, 0.0, 0.5, 0.0,
        0.5, 0.5, 0.5, 1.0
    );
    
    glm::mat4 depthBiasPV = biasMatrix * depthProj * depthView;
    shader.loadDepth(depthBiasPV);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, reflectionTexture);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, shadowMap);

    for(size_t i = 0; i < entities.size(); ++i){
        shader.loadEntity(entities[i]);
        if(entities[i]->getModel() != NULL){
            renderModel(entities[i]->getModel());
        } 
    }

    shader.disable();
}

void EntityRenderer::renderModel(Model* model){
    std::vector<ModelComponent>* components = model->getModelComponents();
    for(size_t i = 0; i < components->size(); ++i){
        ModelComponent current = components->at(i);

        shader.loadModelComponent(current);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, current.getTextureID());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glBindVertexArray(current.getVaoID());

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        
        glDrawElements(GL_TRIANGLES, current.getIndexCount(), GL_UNSIGNED_INT, (void*)0);
        
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        glBindVertexArray(0);
    }
}