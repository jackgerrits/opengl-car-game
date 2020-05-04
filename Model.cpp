#include "Model.h"

#include <algorithm>
#include <stdexcept>
#include <cfloat>

// Taken from tiny_obj_loader.h as its hidden in a different namespace in the implementation.
void initMaterial(tinyobj::material_t& material) {
    material.name = "";
    material.ambient_texname = "";
    material.diffuse_texname = "";
    material.specular_texname = "";
    material.specular_highlight_texname = "";
    material.bump_texname = "";
    material.displacement_texname = "";
    material.alpha_texname = "";
    for (int i = 0; i < 3; i++) {
        material.ambient[i] = 0.f;
        material.diffuse[i] = 0.f;
        material.specular[i] = 0.f;
        material.transmittance[i] = 0.f;
        material.emission[i] = 0.f;
    }
    material.illum = 0;
    material.dissolve = 1.f;
    material.shininess = 1.f;
    material.ior = 1.f;
    material.unknown_parameter.clear();
}

ModelComponent::ModelComponent(GLuint vaoID, size_t indexCount, GLuint textureID, const tinyobj::material_t& material) {
    this->vaoID = vaoID;
    this->indexCount = indexCount;
    this->textureID = textureID;
    this->material = material;
}
ModelComponent::ModelComponent(GLuint vaoID, size_t indexCount, GLuint textureID) {
    this->vaoID = vaoID;
    this->indexCount = indexCount;
    this->textureID = textureID;
    this->material.specular[0] = 1.0f;
    this->material.specular[1] = 0.0f;
    this->material.specular[2] = 1.0f;
    initMaterial(this->material);
}
ModelComponent::ModelComponent() {
    this->vaoID = 0;
    this->indexCount = 0;
    this->textureID = 0;
    initMaterial(this->material);
}

size_t ModelComponent::getIndexCount() const {
    return indexCount;
}

GLuint ModelComponent::getVaoID() const {
    return vaoID;
}

GLuint ModelComponent::getTextureID() const {
    return textureID;
}

tinyobj::material_t ModelComponent::getMaterial() const {
    return material;
}

Model::Model(const std::vector<ModelComponent>& components) {
    this->components = components;
    for (int i = 0; i < 3; ++i) {
        maxRanges.push_back(FLT_MAX);
        maxRanges.push_back(-FLT_MAX);
    }
}

// Adds the vertices into the range stored for this model.
void Model::addRange(const std::vector<float>& vertices) {
    for (int dim = 0; dim < 3; ++dim) {
        for (size_t j = dim; j < vertices.size(); j += 3) {
            maxRanges[2 * dim] = std::min(vertices[j], maxRanges[2 * dim]);
            maxRanges[2 * dim + 1] = std::max(vertices[j], maxRanges[2 * dim + 1]);
        }
    }
}

// [xMin, xMax, yMin, yMax, zMin, zMax]
void Model::setRange(
    const std::tuple<float, float, float>& /*minXYZ*/, const std::tuple<float, float, float>& /*maxXYZ*/) {
    throw std::runtime_error("Not implemented");
}

std::pair<float, float> Model::getRangeInDim(int dim) const {
    return std::make_pair(maxRanges[2 * dim], maxRanges[2 * dim + 1]);
}

Model::Model() {
    for (int i = 0; i < 3; ++i) {
        maxRanges.push_back(FLT_MAX);
        maxRanges.push_back(-FLT_MAX);
    }
}

void Model::addModelComponent(const ModelComponent& component) {
    components.push_back(component);
}

const std::vector<ModelComponent>& Model::getModelComponents() const {
    return components;
}
