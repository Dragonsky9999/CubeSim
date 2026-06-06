#pragma once
#include <vector>
#include "src/Cube/core/cubePiece.h"

struct CubeState {
    unsigned int N;
    std::vector<Piece> pieces;

    CubeState(unsigned int n = 3);
    void initCube();

    void reset();
};