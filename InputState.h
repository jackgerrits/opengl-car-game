/**
    A set of variables that records mouse activity.
    You can update the variables yourself or use the functions.

    This code is reused from the example.
*/

#ifndef _INPUTSTATE_H_
#define _INPUTSTATE_H_

#define _USE_MATH_DEFINES

struct InputState {
    InputState() : lMousePressed(false),
        rMousePressed(false),
        prevX(0), prevY(0),
        deltaX(0), deltaY(0),
        scrollDeltaX(0), scrollDeltaY(0) {}

    // Is the mouse button currently being held down?
    bool lMousePressed;
    bool rMousePressed;

    // Last known position of the cursor
    float prevX;
    float prevY;

    // Accumulated change in cursor position. 
    float deltaX;
    float deltaY;

    float scrollDeltaX;
    float scrollDeltaY;

    // Update cursor variables based on new position x,y
    void update(float x, float y) {
        float xDiff = x - prevX;
        float yDiff = y - prevY;
        deltaX += xDiff;
        deltaY += yDiff;
        prevX = x;
        prevY = y;
    };

    void updateScroll(float x, float y) {
        scrollDeltaX += x;
        scrollDeltaY += y;
    };

    // Read off the accumulated motion and reset it
    void readDeltaAndReset(float* x, float* y) {
        *x = deltaX;
        *y = deltaY;
        deltaX = 0;
        deltaY = 0;
    };
};

#endif // _INPUTSTATE_H_
