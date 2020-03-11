#ifndef TERRAINSHADER_H
#define TERRAINSHADER_H

#define _USE_MATH_DEFINES

#include "../entities/Terrain.h"
#include "../entities/Light.h"
#include "../entities/Camera.h"
#include "../Model.h"
#include "ShaderProgram.h"

#include <cstdio>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include <glm/glm.hpp>
#pragma warning(push, 0)
#include <glm/ext.hpp>
#pragma warning(pop)
#include <GL/glew.h>

const std::string TERRAIN_VERTEX_SHADER = "shaders/terrain.vert";
const std::string TERRAIN_FRAGMENT_SHADER = "shaders/terrain.frag";

class TerrainShader : public ShaderProgram {
private:
    GLuint location_blendMap;
    GLuint location_backMap;
    GLuint location_rMap;
    GLuint location_gMap;
    GLuint location_bMap;
    GLuint location_shadowMap;

    GLuint location_clip_plane;

    GLuint location_projection;
    GLuint location_model;
    GLuint location_view;

    GLuint location_depth_pv;

    GLuint location_num_lights;
    GLuint location_shininess;
public:
    TerrainShader();

    virtual void bindUniformLocations();

    void loadTerrain(Terrain* terrain);

    void loadLights(std::vector<Light*> lights);
    void loadLight(Light* light, int i);
    void loadView(glm::mat4 view);
    void loadDepth(glm::mat4 pv);
    void loadClipPlane(glm::vec4 clip);
    void loadProjection(glm::mat4 proj);
};

#endif //TERRAINSHADER_H
