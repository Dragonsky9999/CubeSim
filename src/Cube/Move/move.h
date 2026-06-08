#pragma once

#include "src/Cube/face/face.h"

struct Move {
    Axis axis;
    int startLayer;
    int endLayer;
    int amount;
    int dir;
};