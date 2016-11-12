#ifndef GAMETIME_H
#define GAMETIME_H

#define _USE_MATH_DEFINES

#include <GLFW/glfw3.h>

class GameTime {
private:
    static GameTime* gameTime;
    GameTime();

    double initialTime;
    double lastTime;
    double currentTime;
public:
    static GameTime* getGameTime();

    void update();  // Should be called once per frame
    float getDt();
    float getFPS();
};

#endif
