#include "GameTime.h"

#include <GLFW/glfw3.h>

// Initialise singleton
GameTime* GameTime::gameTime = nullptr;

GameTime::GameTime() {
    initialTime = glfwGetTime();
    lastTime = -1;
    currentTime = -1;
}

GameTime* GameTime::getGameTime() {
    if (gameTime == nullptr) {
        gameTime = new GameTime();
    }

    return gameTime;
}

void GameTime::update() {
    lastTime = currentTime;
    currentTime = glfwGetTime();
}

float GameTime::getDt() const {
    return static_cast<float>(currentTime - lastTime);
}

float GameTime::getFPS() const {
    return static_cast<float>(1.f / getDt());
}
