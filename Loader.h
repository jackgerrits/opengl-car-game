#pragma once

#include "Model.h"
#include <tiny_obj_loader.h>

#include <sys/stat.h>
#include <glad/glad.h>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <glm/glm.hpp>

struct Image {
    unsigned char* data;
    int width;
    int height;
    int channels;

    Image();
    Image(unsigned char* data, int width, int height, int channels);
    glm::vec3 getPixel(int x, int y) const;
};

class Loader {
  private:
    static Loader* loader;
    Loader() = default;

    // Stores the file/id mapping for each loaded texture to use for caching.
    std::map<std::string, GLuint> loadedTextures;
    static GLuint loadTextureData(GLubyte* data, int x, int y, int n, GLenum textureUnit);
    static GLuint setupBuffer(unsigned int buffer, const std::vector<float>& values, int attributeIndex, int dataDimension);
    static GLuint setupIndicesBuffer(unsigned int buffer, const std::vector<unsigned int>& values);

  public:
    static Loader* getLoader();

    static std::vector<float> generateNormals(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);

    static bool fileExists(const std::string& name);
    Model loadModel(const std::string& filepath);
    Model loadModel(const std::vector<tinyobj::shape_t>& shapes, const std::vector<tinyobj::material_t>& materials,
        const std::string& materialpath);
    ModelComponent loadModelComponent(
        const tinyobj::shape_t&, const std::vector<tinyobj::material_t>& materials, const std::string& materialpath);
    ModelComponent loadModelComponent(const std::vector<float>& vertices, const std::vector<unsigned int>& indices,
        const std::vector<float>& texCoords);
    ModelComponent loadModelComponent(const std::vector<float>& vertices, const std::vector<unsigned int>& indices,
        const std::vector<float>& texCoords, const std::vector<float>& normals);
    ModelComponent loadModelComponent(const std::vector<float>& vertices, const std::vector<unsigned int>& indices,
        const std::vector<float>& texCoords, const std::string& texturepath);
    ModelComponent loadModelComponent(const std::vector<float>& vertices, const std::vector<unsigned int>& indices,
        const std::vector<float>& texCoords, const std::vector<float>& normals, const std::string& texturepath);

    static GLuint loadVAO(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
    static GLuint loadVAO(const std::vector<float>& vertices, const std::vector<unsigned int>& indices,
        const std::vector<float>& texCoords);
    static GLuint loadVAO(const std::vector<float>& vertices, const std::vector<unsigned int>& indices,
        const std::vector<float>& texCoords, const std::vector<float>& normals);
    GLuint loadVAO(const tinyobj::shape_t&);

    static Image loadImage(const std::string& filepath);
    static GLuint loadCubemapTexture(const std::vector<std::string>& filenames);
    GLuint loadTexture(const std::string& filepath);
    GLuint loadDefaultTexture();
};
