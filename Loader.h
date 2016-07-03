#ifndef LOADER_H
#define LOADER_H

#include "Model.h"
#include "libs/stb_image.h"
#include "libs/tiny_obj_loader.h"

#include <sys/stat.h>
#include <GL/glew.h>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <glm/glm.hpp>

struct Image {
    unsigned char *data;
    int width;
    int height;
    int channels;

    Image();
    Image(unsigned char* data, int width, int height, int channels);
    glm::vec3 getPixel(int x, int y);
};

class Loader {
private:
    static Loader* loader;
    Loader();

    std::map<std::string, GLuint> loadedTextures;
    GLuint loadTextureData(GLubyte *data, int x, int y, int n, GLenum textureUnit);
    GLuint setupBuffer(unsigned int buffer, std::vector<float> values, int attributeIndex, int dataDimension);
    GLuint setupIndicesBuffer(unsigned int buffer, std::vector<unsigned int> values);
public:
    static Loader* getLoader();

    std::vector<float> generateNormals(std::vector<float> vertices, std::vector<unsigned int> indices);

    bool fileExists(const std::string& name);
    Model loadModel(std::string filepath);
    Model loadModel(std::vector<tinyobj::shape_t> shapes, std::vector<tinyobj::material_t> materials, std::string materialpath);
    ModelComponent loadModelComponent(tinyobj::shape_t, std::vector<tinyobj::material_t> materials, std::string materialpath);
    ModelComponent loadModelComponent(std::vector<float> vertices, std::vector<unsigned int> indices, std::vector<float> texCoords);
    ModelComponent loadModelComponent(std::vector<float> vertices, std::vector<unsigned int> indices, std::vector<float> texCoords, std::vector<float> normals);
    ModelComponent loadModelComponent(std::vector<float> vertices, std::vector<unsigned int> indices, std::vector<float> texCoords, std::string texturepath);
    ModelComponent loadModelComponent(std::vector<float> vertices, std::vector<unsigned int> indices, std::vector<float> texCoords, std::vector<float> normals, std::string texturepath);

    GLuint loadVAO(std::vector<float> vertices, std::vector<unsigned int> indices);
    GLuint loadVAO(std::vector<float> vertices, std::vector<unsigned int> indices, std::vector<float> texCoords);
    GLuint loadVAO(std::vector<float> vertices, std::vector<unsigned int> indices, std::vector<float> texCoords, std::vector<float> normals);
    GLuint loadVAO(tinyobj::shape_t);
    
    Image loadImage(std::string filepath);
    GLuint loadCubemapTexture(std::vector<std::string> filenames);
    GLuint loadTexture(std::string filepath);
    GLuint loadDefaultTexture();
};

#endif