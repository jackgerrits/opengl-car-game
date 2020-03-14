#ifndef TERRAIN_H
#define TERRAIN_H

#define _USE_MATH_DEFINES

#include <cmath>

#include "../Model.h"
#include "Entity.h"
#include "../Loader.h"

#include <assert.h>
#include <string>
#include <iostream>
#include <GL/glew.h>
#pragma warning(push, 0)
#include <glm/ext.hpp>
#pragma warning(pop)

class Terrain : public Entity{
protected:
    std::vector<GLuint> textures;
    Image heightMap;

public:
    static const float TERRAIN_SIZE;
    static const float TERRAIN_MAX_HEIGHT;

    Terrain(Model* model, std::vector<GLuint> textures, Image heightMap);
    static Terrain* loadTerrain(std::vector<std::string> images, std::string heightMapFile);
    static Model* generateTerrainModel(Image heightMap);

    GLuint getVaoID();
    size_t getIndexCount();
    GLuint getTextureID(int);
    bool isOnTerrain(float x, float z);
    float getHeight(float x, float z);
    float getHeight(int x, int z);
    int convertCoordinate(float coord);

    glm::vec3 getPositionFromPixel(int x, int y);

    float getAngleX(float x, float y, float rotation);
    float getAngleZ(float x, float y, float rotation);
    float getAngle(float x, float y, float rotation, float offset);
};




#endif