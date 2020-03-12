#ifndef ENTITY_H
#define ENTITY_H

#define _USE_MATH_DEFINES

#include "../Model.h"

#include <assert.h>
#include <string>
#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Entity {
protected:
    Model* model;

    glm::vec3 position;
    glm::vec3 scale;
    float xRot;
    float yRot;
    float zRot;

public:
    Entity(Model* model);
    Entity();

    virtual bool update();

    Model* getModel() const;
    glm::mat4 getModelMatrix();

    glm::vec3 getPosition() const;
    glm::vec3 getScale() const;
    float getRotationX() const;
    float getRotationY() const;
    float getRotationZ() const;

    glm::vec3 getDirectionVector();

    // Can be overriden in inheriting class if behaviour requires it.
    // Set the value absolutely of position, scale, or rotation.
    virtual void setPosition(glm::vec3);
    virtual void placeBottomEdge(float surfaceY);
    virtual void setScale(glm::vec3);
    virtual void setRotationX(float);
    virtual void setRotationY(float);
    virtual void setRotationZ(float);


    // Set the value of rotation or position relatively (Takes into account current value)
    virtual void rotateX(float);
    virtual void rotateY(float);
    virtual void rotateZ(float);
    virtual void move(glm::vec3);

    // Generates the transformation to be applied to the mesh with the given parameters.
    static glm::mat4 calculateModelMatrix(glm::vec3 position, glm::mat4 rotation, glm::vec3 scale);
    static glm::mat4 calculateRotationMatrix(float xRot, float yRot, float zRot);
};


#endif