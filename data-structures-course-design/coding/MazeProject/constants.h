#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QtGlobal>

const int DEFAULT_SIZE = 20;

inline int& getMaxX() {
    static int value = 20;
    return value;
}

inline int& getMaxY() {
    static int value = 20;
    return value;
}

inline int& getGridSize() {
    static int value = 25;
    return value;
}

#define MAX_X (getMaxX())
#define MAX_Y (getMaxY())
#define gridSize (getGridSize())

const int startX = 20;
const int startY = 20;

const double MOVE_COST_STRAIGHT = 1.0;
const double MOVE_COST_DIAGONAL = 1.4;

const int MOUSE_MOVE_THROTTLE_MS = 30;

#endif
