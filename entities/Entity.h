#pragma once

#define _USE_MATH_DEFINES

#include "../Model.h"

#include <cassert>
#include <string>
#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Entity {
  protected:
    const Model* m_model;

    glm::vec3 m_position;
    glm::vec3 m_scale;
    float m_x_rot;
    float m_y_rot;
    float m_z_rot;

  public:
    Entity(const Model* model);
    Entity();

    virtual ~Entity() = default;

    virtual bool update();

    [[nodiscard]] const Model* getModel() const;
    [[nodiscard]] glm::mat4 calculateModelMatrix() const;
    [[nodiscard]] const glm::vec3& getPosition() const;
    [[nodiscard]] const glm::vec3& getScale() const;
    [[nodiscard]] float getRotationX() const;
    [[nodiscard]] float getRotationY() const;
    [[nodiscard]] float getRotationZ() const;

    [[nodiscard]] glm::vec3 calculateDirectionVector() const;

    // Can be overridden in inheriting class if behaviour requires it.
    // Set the value absolutely of position, scale, or rotation.
    virtual void setPosition(const glm::vec3&);
    virtual void placeBottomEdge(float surfaceY);
    virtual void setScale(const glm::vec3&);
    virtual void setRotationX(float);
    virtual void setRotationY(float);
    virtual void setRotationZ(float);

    // Set the value of rotation or position relatively (Takes into account current value)
    virtual void rotateX(float);
    virtual void rotateY(float);
    virtual void rotateZ(float);
    virtual void move(const glm::vec3&);

    // Generates the transformation to be applied to the mesh with the given parameters.
    static glm::mat4 calculateModelMatrix(const glm::vec3& position, const glm::mat4& rotation, const glm::vec3& scale);
    static glm::mat4 calculateRotationMatrix(float xRot, float yRot, float zRot);
};
