#include "Entity.h"

using namespace std;

// Constructor accepts a model defining vertex, colour and index data for this entity.
Entity::Entity(Model* model){
    this->model = model;

    position = glm::vec3(0.0f, 0.0f, 0.0f);
    scale = glm::vec3(1.0f, 1.0f, 1.0f);
    xRot = 0.0f;
    yRot = 0.0f;
    zRot = 0.0f;
}

Entity::Entity(){
    this->model = NULL;

    position = glm::vec3(0.0f, 0.0f, 0.0f);
    scale = glm::vec3(1.0f, 1.0f, 1.0f);
    xRot = 0.0f;
    yRot = 0.0f;
    zRot = 0.0f;
}

bool Entity::update(){
    return false;
}

Model* Entity::getModel() const {
    return model;
}

glm::mat4 Entity::getModelMatrix(){
    glm::mat4 rotation = calculateRotationMatrix(xRot, yRot, zRot);
    return calculateModelMatrix(position, rotation, scale);
}


// Getters and setters for entity state values.
glm::vec3 Entity::getPosition() const {
    return position;
}

glm::vec3 Entity::getScale() const {
    return scale;
}

float Entity::getRotationX() const{
    return xRot;
}

float Entity::getRotationY() const{
    return yRot;
}

float Entity::getRotationZ() const{
    return zRot;
}

glm::vec3 Entity::getDirectionVector(){
    return glm::normalize(glm::vec3(glm::sin(yRot), glm::sin(xRot), glm::cos(yRot)));
}

void Entity::setPosition(glm::vec3 position){
    this->position = position;
}

void Entity::placeBottomEdge(float surfaceY){
    if(model != NULL){
        position.y = surfaceY - model->getRangeInDim(1).first * scale.y;
    }
}

void Entity::setScale(glm::vec3 scale){
    this->scale = scale;
}

void Entity::setRotationX(float rot){
    xRot = rot;
}

void Entity::setRotationY(float rot){
    yRot = rot;
}

void Entity::setRotationZ(float rot){
    zRot = rot;
}
// Set the value of rotation or position relatively (Takes into account current value)
void Entity::rotateX(float rot){
    xRot += rot;
}

void Entity::rotateY(float rot){
    yRot += rot;
}

void Entity::rotateZ(float rot){
    zRot += rot;
}

void Entity::move(glm::vec3 movement){
    position = position + movement;
}

glm::mat4 Entity::calculateModelMatrix(glm::vec3 position, glm::mat4 rotationMat, glm::vec3 scale) {
    glm::mat4 modelMatrix(1.0f);

    // scale, rotate and translate
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = modelMatrix * rotationMat;
    modelMatrix = glm::scale(modelMatrix, scale);

    return modelMatrix;
}

glm::mat4 Entity::calculateRotationMatrix(float xRot, float yRot, float zRot) {
    glm::mat4 rotation(1.0f);

    rotation = glm::rotate(rotation, yRot, glm::vec3(0.0f, 1.0f, 0.0f));
    rotation = glm::rotate(rotation, xRot, glm::vec3(1.0f, 0.0f, 0.0f));
    rotation = glm::rotate(rotation, zRot, glm::vec3(0.0f, 0.0f, 1.0f));

    return rotation;
}
