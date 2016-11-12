#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#define _USE_MATH_DEFINES

#include "EntityRenderer.h"
#include "TerrainRenderer.h"
#include "SkyboxRenderer.h"
#include "../Model.h"
#include "../ShadowMap.h"
#include "../entities/Entity.h"
#include "../entities/Light.h"
#include "../entities/Camera.h"
#include "../entities/Player.h"
#include "../entities/Terrain.h"
#include "../FrameBuffer.h"
#include "../particles/Particle.h"
#include "../particles/ParticleManager.h"
#include "../particles/ParticleSystem.h"
#include "../water/WaterRenderer.h"

#include <cstdio>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <GL/glew.h>

const float WATER_PLANE_HEIGHT = 0.398918f;

class RenderManager {
private:
    FrameBuffer reflectionBuffer;
    FrameBuffer refractionBuffer;

    EntityRenderer renderer;
    TerrainRenderer terrainRenderer;
    WaterRenderer waterRenderer;

public:
    RenderManager();

    // Renders the entire scene, including 4 passes (shadow, refraction, reflection, total)
    // Lots of parameters, some of these could be maybe moved to the constructor as they do not change.
    // Not sure what a better way to manage all of these paramters is
    void render(const std::vector<Entity*>& entities, const std::vector<Light*>& lights, Terrain* terrain, Entity* water, SkyboxRenderer& skybox, ShadowMap shadowMap, Camera* cam, const glm::mat4& projection, const int winX, const int winY);
};

#endif //RENDER_MANAGER_H

