#pragma once

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
    float getDt() const;
    float getFPS() const;
};
