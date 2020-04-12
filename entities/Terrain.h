#pragma once

#include "../Model.h"
#include "Entity.h"
#include "../Loader.h"

#include <string>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Terrain : public Entity {
  protected:
    std::vector<GLuint> textures;
    Image heightMap;

  public:
    static const float TERRAIN_SIZE;
    static const float TERRAIN_MAX_HEIGHT;

    Terrain(Model* model, const std::vector<GLuint> &textures, Image heightMap);
    static Terrain* loadTerrain(const std::vector<std::string>& images, const std::string& heightMapFile);
    static Model* generateTerrainModel(const Image& heightMap);

    GLuint getVaoID() const;
    size_t getIndexCount() const;
    GLuint getTextureID(int) const;
    bool isOnTerrain(float x, float z) const;
    float getHeight(float x, float z) const;
    float getHeight(int x, int z) const;
    int convertCoordinate(float coord) const;

    glm::vec3 getPositionFromPixel(int x, int y) const;

    float getAngleX(float x, float y, float rotation) const;
    float getAngleZ(float x, float y, float rotation) const;
    float getAngle(float x, float y, float rotation, float offset) const;
};
