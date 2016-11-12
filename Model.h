#ifndef MODEL_H
#define MODEL_H

#define _USE_MATH_DEFINES

#include "libs/tiny_obj_loader.h"

#include <vector>
#include <cfloat>
#include <utility>
#include <algorithm>
#include <iostream>

#include <GL/glew.h>

void initMaterial(tinyobj::material_t &material);

// Represents a single mesh/shape/vao
class ModelComponent {
private:
    GLuint vaoID;
    int indexCount;
    tinyobj::material_t material;

public:
    GLuint textureID;
    ModelComponent(GLuint, int, GLuint, tinyobj::material_t);
    ModelComponent(GLuint, int, GLuint);
    ModelComponent();

    int getIndexCount() const;
    GLuint getVaoID() const;
    GLuint getTextureID() const;
    tinyobj::material_t getMaterial() const;
};

// Represents a grouping of meshes/shapes/vaos/ModelComponents to form a larger object.
class Model {
private:
    std::vector<ModelComponent> components;
    std::vector<float> maxRanges;   // Essentially forms a bounding box for the model. Format: [xMin, xMax, yMin, yMax, zMin, zMax]
public:
    Model(std::vector<ModelComponent>);
    Model();
    void addModelComponent(ModelComponent);
    std::vector<ModelComponent>* getModelComponents();

    void addRange(std::vector<float> vertices);
    std::pair<float, float> getRangeInDim(int dim);
};

#endif
