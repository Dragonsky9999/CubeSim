#pragma once

#include "src/Cube/face/face.h"

struct Move {
    Axis axis;
    int layer;
    int width;
    int amount;
};