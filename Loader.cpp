#include "Loader.h"

#include <stb_image.h>

#define VALS_PER_VERT 3
#define VALS_PER_NORMAL 3
#define VALS_PER_TEX 2

Image Image::loadFromFile(std::string_view filePath) {
    Image image;
    const std::string definitelyNullTerminatedFilePath(filePath);
    image.data = stbi_load(definitelyNullTerminatedFilePath.c_str(),  // char* filepath
        &image.width,                                                 // The address to store the width of the image
        &image.height,                                                // The address to store the height of the image
        &image.channels,                                              // Number of channels in the image
        0                                                             // Force number of channels if > 0
    );
    return image;
}

Image::Image() : data(nullptr), width(-1), height(-1), channels(-1) {
}

Image::~Image() {
    stbi_image_free(data);
}

Image::Image(unsigned char* data, int width, int height, int channels)
    : data(data), width(width), height(height), channels(channels) {
}

glm::vec3 Image::getPixel(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return glm::vec3(-1.0f, -1.0f,
            -1.0f);  // Returns a vector of -1 to indicate the image does not contain a pixel at that location.
    }
    int offset =
        ((width * y) + x) * channels;  // Determine the position in data to start reading. Each pixel is 1 byte.
    return glm::vec3((float)data[offset] / 255, (float)data[offset + 1] / 255, (float)data[offset + 2] / 255);
}

// Initialise Loader singleton
Loader* Loader::loader = nullptr;

Loader* Loader::getLoader() {
    if (loader == nullptr) {
        loader = new Loader();
    }

    return loader;
}

glm::vec3 getVertex(const std::vector<float>& vertices, int index) {
    int pos = index * 3;
    return glm::vec3(vertices[pos], vertices[pos + 1], vertices[pos + 2]);
}

std::vector<float> Loader::generateNormals(
    const std::vector<float>& vertices, const std::vector<unsigned int>& indices) {
    std::vector<float> resultNormals(vertices.size(), 0.0f);

    // Iterate over each triangle as defined in indices
    for (size_t i = 0; i < indices.size(); i += 3) {
        glm::vec3 vert1 = getVertex(vertices, indices[i]);
        glm::vec3 vert2 = getVertex(vertices, indices[i + 1]);
        glm::vec3 vert3 = getVertex(vertices, indices[i + 2]);

        glm::vec3 v1 = vert2 - vert1;
        glm::vec3 v2 = vert3 - vert1;

        glm::vec3 faceNormal = glm::cross(v1, v2);
        faceNormal = glm::normalize(faceNormal);

        // Iterate over each vertex in that triangle
        for (size_t j = i; j < i + 3; j++) {
            glm::vec3 vertNormal(resultNormals[indices[j] * 3], resultNormals[(indices[j] * 3) + 1],
                resultNormals[(indices[j] * 3) + 2]);

            // Add the current faces normal to the vertexes normal
            vertNormal = glm::normalize(vertNormal + faceNormal);

            resultNormals[indices[j] * 3] = vertNormal.x;
            resultNormals[(indices[j] * 3) + 1] = vertNormal.y;
            resultNormals[(indices[j] * 3) + 2] = vertNormal.z;
        }
    }

    return resultNormals;
}

// http://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
inline bool Loader::fileExists(const std::string& name) {
    struct stat buffer{};
    return (stat(name.c_str(), &buffer) == 0);
}

Model Loader::loadModel(const std::string& model_file) {
    // Declare containers for object values
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    // If the object file is in a different directory, the material file path must be specified.
    // Assumes material file is in the same directory as obj
    std::string model_file_path;
    if (model_file.find('/') != std::string::npos) {
        model_file_path = model_file.substr(0, model_file.find_last_of("\\/") + 1);
    }

    // Load object
    std::string err;
    bool succeeded = tinyobj::LoadObj(shapes, materials, err, model_file.c_str(), model_file_path.c_str());
    if (!err.empty()) {
        std::cerr << err << std::endl;
    }
    if (!succeeded) {
        exit(1);
    }

    return loadModel(shapes, materials, model_file_path);
}

Model Loader::loadModel(const std::vector<tinyobj::shape_t>& shapes, const std::vector<tinyobj::material_t>& materials,
    const std::string& materialpath) {
    Model model;
    for (const auto& shape : shapes) {
        ModelComponent component = loadModelComponent(shape, materials, materialpath);
        model.addRange(shape.mesh.positions);
        model.addModelComponent(component);
    }
    return model;
}

ModelComponent Loader::loadModelComponent(
    const tinyobj::shape_t& shape, const std::vector<tinyobj::material_t>& materials, const std::string& materialpath) {
    GLuint vao = loadVAO(shape);
    size_t numIndices = shape.mesh.indices.size();

    // TODO - revisit this. Likely a result of the file not loading on windows requiring this, meaning no textures can
    // load.
    tinyobj::material_t material;
    initMaterial(material);
    if (!shape.mesh.material_ids.empty() && shape.mesh.material_ids[0] != -1) {
        material = materials[shape.mesh.material_ids[0]];  // Loads the first material
    }
    GLuint textureID = loadTexture(materialpath + material.diffuse_texname);

    return ModelComponent(vao, numIndices, textureID, material);
}

ModelComponent Loader::loadModelComponent(
    const std::vector<float>& vertices, const std::vector<unsigned int>& indices, const std::vector<float>& texCoords) {
    GLuint vao = loadVAO(vertices, indices, texCoords);
    size_t numIndices = indices.size();
    GLuint textureID = loadDefaultTexture();

    return ModelComponent(vao, numIndices, textureID);
}

ModelComponent Loader::loadModelComponent(const std::vector<float>& vertices, const std::vector<unsigned int>& indices,
    const std::vector<float>& texCoords, const std::vector<float>& normals) {
    GLuint vao = loadVAO(vertices, indices, texCoords, normals);
    size_t numIndices = indices.size();
    GLuint textureID = loadDefaultTexture();

    return ModelComponent(vao, numIndices, textureID);
}

ModelComponent Loader::loadModelComponent(const std::vector<float>& vertices, const std::vector<unsigned int>& indices,
    const std::vector<float>& texCoords, const std::string& texturepath) {
    GLuint vao = loadVAO(vertices, indices, texCoords);
    size_t numIndices = indices.size();
    GLuint textureID = loadTexture(texturepath);

    return ModelComponent(vao, numIndices, textureID);
}

ModelComponent Loader::loadModelComponent(const std::vector<float>& vertices, const std::vector<unsigned int>& indices,
    const std::vector<float>& texCoords, const std::vector<float>& normals, const std::string& texturepath) {
    GLuint vao = loadVAO(vertices, indices, texCoords, normals);
    size_t numIndices = indices.size();
    GLuint textureID = loadTexture(texturepath);

    return ModelComponent(vao, numIndices, textureID);
}

GLuint Loader::loadVAO(
    const std::vector<float>& vertices, const std::vector<unsigned int>& indices, const std::vector<float>& texCoords) {
    GLuint vaoHandle;
    glGenVertexArrays(1, &vaoHandle);
    glBindVertexArray(vaoHandle);

    unsigned int buffer[3];
    glGenBuffers(3, buffer);

    setupBuffer(buffer[0], vertices, 0, VALS_PER_VERT);
    setupBuffer(buffer[1], texCoords, 1, VALS_PER_TEX);
    setupIndicesBuffer(buffer[2], indices);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return vaoHandle;
}

GLuint Loader::loadVAO(const std::vector<float>& vertices, const std::vector<unsigned int>& indices) {
    GLuint vaoHandle;
    glGenVertexArrays(1, &vaoHandle);
    glBindVertexArray(vaoHandle);

    unsigned int buffer[2];
    glGenBuffers(2, buffer);

    setupBuffer(buffer[0], vertices, 0, VALS_PER_VERT);
    setupIndicesBuffer(buffer[1], indices);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return vaoHandle;
}

GLuint Loader::loadVAO(const std::vector<float>& vertices, const std::vector<unsigned int>& indices,
    const std::vector<float>& texCoords, const std::vector<float>& normals) {
    GLuint vaoHandle;
    glGenVertexArrays(1, &vaoHandle);
    glBindVertexArray(vaoHandle);

    unsigned int buffer[4];
    glGenBuffers(4, buffer);

    setupBuffer(buffer[0], vertices, 0, VALS_PER_VERT);
    setupBuffer(buffer[1], normals, 1, VALS_PER_NORMAL);
    setupBuffer(buffer[2], texCoords, 2, VALS_PER_TEX);
    setupIndicesBuffer(buffer[3], indices);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return vaoHandle;
}

GLuint Loader::setupBuffer(
    unsigned int buffer, const std::vector<float>& values, int attributeIndex, int dataDimension) {
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * values.size(), &values[0], GL_STATIC_DRAW);
    glVertexAttribPointer(attributeIndex, dataDimension, GL_FLOAT, GL_FALSE, 0, 0);

    return buffer;
}

GLuint Loader::setupIndicesBuffer(unsigned int buffer, const std::vector<unsigned int>& values) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * values.size(), &values[0], GL_STATIC_DRAW);
    return buffer;
}

GLuint Loader::loadVAO(const tinyobj::shape_t& shape) {
    // If texcoords is null, set it to some dummy values
    if (shape.mesh.texcoords.empty()) {
        auto shapeCopy = shape;
        std::vector<float> texVec;
        for (size_t i = 0; i < shapeCopy.mesh.positions.size(); i += 3) {
            texVec.push_back(0.0f);
            texVec.push_back(0.0f);
        }
        shapeCopy.mesh.texcoords = texVec;
        // Call this function again with a shape that has textcoords.
        return loadVAO(shapeCopy);
    }

    return loadVAO(shape.mesh.positions, shape.mesh.indices, shape.mesh.texcoords, shape.mesh.normals);
}

GLuint Loader::loadCubemapTexture(const std::vector<std::string>& filenames) {
    if (filenames.size() != 6) {
        std::cerr << "[Loader][Error] Cubemap requires 6 texture files." << std::endl;
        exit(1);
    }

    GLuint textureID;
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    for (size_t i = 0; i < filenames.size(); i++) {
        std::cout << "[Loader] loading: " << filenames[i] << std::endl;
        if (!fileExists(filenames[i])) {
            std::cerr << "[Loader][Error] Skybox texture file " << i << " doesnt exist." << std::endl;
        }

        Image image = Image::loadFromFile(filenames[i]);

        GLenum format = GL_RGB;
        if (image.channels == 4) {
            format = GL_RGBA;
        }

        // The first cast works because the enums are defined sequentially.
        glTexImage2D(static_cast<GLenum>(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i), 0, GL_RGB, image.width, image.height, 0,
            format, GL_UNSIGNED_BYTE, image.data);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }

    return textureID;
}

GLuint Loader::loadTexture(const std::string& filepath) {
    if (loadedTextures.count(filepath)) {
        std::cout << "[Loader] '" << filepath << "' already loaded, using cached texture." << std::endl;
        return loadedTextures[filepath];
    }

    std::cout << "[Loader] loading: " << filepath << std::endl;
    if (!fileExists(filepath)) {
        std::cerr << "[Loader] File doesnt exist, loading default texture." << std::endl;
        return loadDefaultTexture();
    }

    // Load an image from file as texture
    const Image image = Image::loadFromFile(filepath);

    GLuint textureID = loadTextureData(image.data, image.width, image.height, image.channels, GL_TEXTURE0);

    // Save texture to cache
    loadedTextures[filepath] = textureID;

    return textureID;
}

GLuint Loader::loadTextureData(GLubyte* data, int x, int y, int n, GLenum textureUnit) {
    GLuint textureID;

    glActiveTexture(textureUnit);
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    GLenum format = GL_RGB;

    // If there are four channels include alpha
    if (n == 4) {
        format = GL_RGBA;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, format, GL_UNSIGNED_BYTE, data);

    // Set texture parameters
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    return textureID;
}

GLuint Loader::loadDefaultTexture() {
    if (loadedTextures.count("DEFAULT_TEXTURE")) {
        std::cout << "[Loader] 'DEFAULT_TEXTURE' already loaded, using cached texture." << std::endl;
        return loadedTextures["DEFAULT_TEXTURE"];
    }

    const int SIZE = 64;
    GLubyte myimage[SIZE][SIZE][3];

    // Create checkerboard image as the default texture
    for (size_t i = 0; i < SIZE; i++) {
        for (size_t j = 0; j < SIZE; j++) {
            GLubyte c;
            c = (((i & 0x8) == 0) ^ ((j & 0x8) == 0)) * 255;
            myimage[i][j][0] = c;
            myimage[i][j][1] = c;
            myimage[i][j][2] = c;
        }
    }

    GLuint textureID = loadTextureData(&myimage[0][0][0], SIZE, SIZE, 3, GL_TEXTURE0);
    loadedTextures["DEFAULT_TEXTURE"] = textureID;

    return textureID;
}
