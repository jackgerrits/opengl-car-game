#pragma once

#include <tiny_obj_loader.h>
#include <glad/glad.h>

#include <utility>

void initMaterial(tinyobj::material_t& material);

// Represents a single mesh/shape/vao
class ModelComponent {
  private:
    GLuint vaoID;
    size_t indexCount;
    tinyobj::material_t material;

  public:
    GLuint textureID;
    ModelComponent(GLuint, size_t, GLuint, const tinyobj::material_t&);
    ModelComponent(GLuint, size_t, GLuint);
    ModelComponent();

    size_t getIndexCount() const;
    GLuint getVaoID() const;
    GLuint getTextureID() const;
    tinyobj::material_t getMaterial() const;
};

// Represents a grouping of meshes/shapes/vaos/ModelComponents to form a larger object.
class Model {
  private:
    std::vector<ModelComponent> components;
    // Essentially forms a bounding box for the model. Format: [xMin, xMax, yMin, yMax, zMin, zMax]
    std::vector<float> maxRanges;

  public:
    Model(const std::vector<ModelComponent>&);
    Model();
    void addModelComponent(const ModelComponent&);
    const std::vector<ModelComponent>& getModelComponents() const;

    void addRange(const std::vector<float>& vertices);
    void setRange(const std::tuple<float, float, float>& minXYZ, const std::tuple<float, float, float>& maxXYZ);
    std::pair<float, float> getRangeInDim(int dim) const;
};
