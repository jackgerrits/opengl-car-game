#define GLFW_INCLUDE_NONE
#define _USE_MATH_DEFINES

#include "constants.h"
#include "renderers/RenderManager.h"
#include "GameTime.h"

#include "particles/ParticleManager.h"
#include "particles/ParticleSystem.h"
#include "Window.h"

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "glm_ext.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

using namespace glm;

constexpr float SKYBOX_SIZE = 200.0f;

auto setProjection(int inputWinX, int inputWinY) {
    // float aspect = (float) winX / winY;
    // FOV angle is in radians
    return glm::perspective(constants::PI / 4.0, double(inputWinX) / double(inputWinY), 1.0, 800.0);
}

int main(int argc, char** argv) {
    InputState input;
    glm::mat4 projection;
    Player* player;

    if (argc != 2) {
        std::cerr << "USAGE: " << argv[0] << " basic|physics" << std::endl;
        exit(1);
    }

    // Check if desired controls are basic or physics
    bool basic_controls = strcmp(argv[1], "basic") == 0;

    if (basic_controls) {
        std::cout << "Controls: \n\tw - forward\n\ts - backwards\n\ta/d - turn left/right" << std::endl;
    } else {
        std::cout << "Controls: \n\tw - throttle\n\ts - brake\n\ta/d - steer left/right\n\tspace - handbrake" << std::endl;
    }

    Window window;
    window.set_key_callback([&](GLFWwindow* window, int key, int scancode, int action, int mods) {
        // Terminate program if escape is pressed
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
        player->handleKeyboardEvents(window, key, scancode, action, mods);
    });

    window.set_mouse_position_callback([&](GLFWwindow* /*window*/, double x, double y) {
        input.update((float)x, (float)y);
    });

    window.set_mouse_scroll_callback([&](GLFWwindow* /*window*/, double xoffset, double yoffset) {
        input.updateScroll((float)xoffset, (float)yoffset);
    });

    window.set_mouse_button_callback([&](GLFWwindow* /*window*/, int button, int action, int /*mods*/) {
        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
            input.rMousePressed = true;
        } else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
            input.rMousePressed = false;
        } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            input.lMousePressed = true;
        } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
            input.lMousePressed = false;
        }
    });

    window.set_window_reshape_callback([&](GLFWwindow* /*window*/, int x, int y) {
        projection = setProjection(x, y);
        glViewport(0, 0, x, y);
    });

    srand(static_cast<unsigned int>(time(nullptr)));
    // clang-format off
    // Define skybox textures
    // The current skybox is unfortunately quite low res and has compression issues.
    // However, I wanted a skybox that had no terrain AND a visible sun which was surprisingly hard to find.
    std::vector<std::string> skyboxTextures = {
         "res/sky/sky_right.tga",
         "res/sky/sky_left.tga",
         "res/sky/sky_top.tga",
         "res/sky/sky_bottom.tga",
         "res/sky/sky_back.tga",
         "res/sky/sky_front.tga"
    };
    // clang-format on
    // Load all of the requires models from disk.
    Model barrelModel = Loader::getLoader()->loadModel("res/Barrel/Barrel02.obj");
    Model playerModel = Loader::getLoader()->loadModel("res/car/car-n.obj");
    Model fenceModel = Loader::getLoader()->loadModel("res/fence/fence.obj");
    Model coneModel = Loader::getLoader()->loadModel("res/cone/cone2_obj.obj");
    Model treeModel = Loader::getLoader()->loadModel("res/tree/PineTree03.obj");
    Model stumpModel = Loader::getLoader()->loadModel("res/stump/TreeStump03.obj");

    // Vector to hold all of the world entities.
    std::vector<Entity*> entities;
    // Vector to hold lights
    std::vector<Light*> lights;

    // Create the skybox with the textures defined.
    SkyboxRenderer skybox(skyboxTextures, SKYBOX_SIZE);
    RenderManager manager;

    // Create Terrain using blend map, height map and all of the remaining texture components.
    std::vector<std::string> terrainImages = {"res/terrain/blendMap.png", "res/terrain/grass.jpg", "res/terrain/road.jpg",
        "res/terrain/dirt.png", "res/terrain/mud.jpg"};
    Terrain* terrain = Terrain::loadTerrain(terrainImages, "res/terrain/heightmap.png");
    // Moves the terrain model to be centered about the origin.
    terrain->setPosition(vec3(-Terrain::TERRAIN_SIZE / 2, 0.0f, -Terrain::TERRAIN_SIZE / 2));

    // Load dust texture and create particle system.
    GLuint dust_texture = Loader::getLoader()->loadTexture("./res/dust_single.png");
    ParticleSystem particleSystem(30.0f, 3.0f, 0.5f, 0.5f, dust_texture);

    // Create the player object, scaling for the model, and setting its position in the world to somewhere interesting.
    player = new Player(&playerModel, terrain, basic_controls);
    player->setScale(vec3(0.4f, 0.4f, 0.4f));
    player->setPosition(terrain->getPositionFromPixel(555, 751));
    player->setRotationY((float)5.f * constants::PI / 8.f);
    entities.push_back(player);

    // Initialisation of camera, projection matrix
    projection = setProjection(window.get_width(), window.get_height());
    Camera* cam = new PlayerCamera(player);

    // Create light sources
    auto* sun = new Light();
    sun->position =
        vec4(-1.25 * SKYBOX_SIZE / 10, 2.5 * SKYBOX_SIZE / 10, 3 * SKYBOX_SIZE / 10, 0.0f);  // w = 0 - directional
    sun->specular = vec3(1.0f, 1.0f, 1.0f);
    sun->diffuse = vec3(0.7f, 0.7f, 0.7f);
    sun->ambient = vec3(0.1f, 0.1f, 0.1f);
    lights.push_back(sun);

    auto* headlight = new Light();
    headlight->position = vec4(2.0f, 8.0f, 0.0f, 1.0f);
    headlight->specular = vec3(0.8f, 0.8f, 0.4f);
    headlight->diffuse = vec3(0.8f, 0.8f, 0.4f);
    headlight->coneDirection = vec3(0.0f, -1.0f, 0.0f);
    headlight->coneAngle = constants::PI / 4.f;
    headlight->radius = 10.0f;
    lights.push_back(headlight);

    // Adds entities to random positions on the map
    const size_t RAND_ENTITIES = 500;
    for (size_t i = 0; i < RAND_ENTITIES; i += 2) {
        int selection = rand() % 2;
        Entity* ent;
        switch (selection) {
            case 0:
                ent = new Entity(&treeModel);
                break;
            case 1:
                ent = new Entity(&stumpModel);
                ent->setScale(glm::vec3(0.5, 0.5, 0.5));
                break;
            default:
                throw std::runtime_error("Unreachable statement");
        }

        ent->setPosition(terrain->getPositionFromPixel(rand() % 1024, rand() % 1024));
        entities.push_back(ent);
    }

    // Set of pre calculated cone positions on corners of the track
    // clang-format off
    std::vector<int> conePositions = {
        263, 262, 226, 250, 209, 273,
        213, 299, 342, 717, 329, 734,
        326, 751, 354, 755, 372, 754,
        750, 400, 765, 396, 748, 381,
        828, 480, 842, 476, 854, 478,
        852, 500, 852, 521, 842, 547,
        772, 402
    };
    // clang-format on
    // Creates cones from the positions and adds them.
    for (size_t i = 0; i < conePositions.size(); i += 2) {
        auto* cone = new Entity(&coneModel);
        cone->setPosition(terrain->getPositionFromPixel(conePositions[i], conePositions[i + 1]));
        cone->setScale(vec3(0.01f, 0.01f, 0.01f));  // The cone model was MASSIVE
        entities.push_back(cone);
    }

    // Add the bordering fences to the map.
    float fenceSize = fenceModel.getRangeInDim(0).second - fenceModel.getRangeInDim(0).first;
    for (float x = -Terrain::TERRAIN_SIZE / 2; x < Terrain::TERRAIN_SIZE / 2; x += fenceSize) {
        auto* fence = new Entity(&fenceModel);
        fence->setPosition(vec3(x, 0.0f, Terrain::TERRAIN_SIZE / 2 - 1.0f));
        entities.push_back(fence);

        fence = new Entity(&fenceModel);
        fence->setPosition(vec3(x, 0.0f, -Terrain::TERRAIN_SIZE / 2 + 1.0f));
        entities.push_back(fence);
    }

    for (float z = -Terrain::TERRAIN_SIZE / 2; z < Terrain::TERRAIN_SIZE / 2; z += fenceSize) {
        auto* fence = new Entity(&fenceModel);
        fence->setPosition(vec3(Terrain::TERRAIN_SIZE / 2 - 1.0f, 0.0f, z));
        fence->rotateY(-constants::PI / 2);
        entities.push_back(fence);

        fence = new Entity(&fenceModel);
        fence->setPosition(vec3(-Terrain::TERRAIN_SIZE / 2 + 1.0f, 0.0f, z));
        fence->rotateY(-constants::PI / 2);
        entities.push_back(fence);
    }

    // Goes through each entity and aligns its bottom edge with the terrain at that position.
    for (auto& entity : entities) {
        entity->placeBottomEdge(terrain->getHeight(entity->getPosition().x, entity->getPosition().z));
    }

    // Create the large lake
    auto* water = new Entity();
    water->setScale(vec3(100.0f, 1.0f, 50.0f));
    water->setPosition(terrain->getPositionFromPixel(650, 826));
    water->setPosition(glm::vec3(water->getPosition().x, 0.4f, water->getPosition().z));

    // Create the object for handling rendering to texture for shadows.
    ShadowMap shadowMap(player, lights[0], 4096);

    // Main logic/render loop.
    while (!glfwWindowShouldClose(window.get_window())) {
        GameTime::getGameTime()->update();
        cam->update(input);

        // Render entire scene
        manager.render(entities, lights, terrain, water, skybox, shadowMap, cam, projection, window.get_width(),
            window.get_height());

        // Updates all particles and entities.
        ParticleManager::getParticleManager()->update();
        for (auto* entity : entities) {
            entity->update();
        }

        // Generate dust particles at the players positions if the car is going past enough or moving
        if (player->absVel > 5.0f || player->getThrottle() > 0.1f || (basic_controls && player->getBrake() > 0.1f)) {
            particleSystem.generateParticles(player->getPosition() - player->calculateDirectionVector());
        }

        // Update the position of the car headlights
        headlight->position = vec4(player->getPosition() + vec3(0.0f, 0.1f, 0.0f), 1.0f);
        headlight->coneDirection = player->calculateDirectionVector();

        glFlush();

        glfwSwapBuffers(window.get_window());
        glfwPollEvents();
    }

    // Cleanup program, delete all the dynamic entities.
    delete player;
    delete water;
    for (auto* entity : entities) {
        delete entity;
    }

    glfwDestroyWindow(window.get_window());
    glfwTerminate();

    return 0;
}
