#include "GameTime.h"

// Initialise singleton
GameTime* GameTime::gameTime = NULL;

GameTime::GameTime(){
    initialTime = glfwGetTime();
    lastTime = -1;
    currentTime = -1;
}

GameTime* GameTime::getGameTime(){
    if(gameTime == NULL){
        gameTime = new GameTime();
    }

    return gameTime;
}

void GameTime::update(){
    lastTime = currentTime;
    currentTime = glfwGetTime();
}

float GameTime::getDt(){
    return (float)(currentTime - lastTime);
}

float GameTime::getFPS(){
    return (float)1.0 / getDt();
}
