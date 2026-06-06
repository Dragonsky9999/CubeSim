#pragma once

#include <vector>
#include <string>
#include "src/Cube/cubeState.h"
#include "src/Cube/cubelet.h"
#include "src/Color/color.h"

class Cube {
private:
    COLOR FACE_COLORS[6];
    CubeState STATE;
    std::vector<Cubelet> CUBELETS;
public:
    Cube(int n = 3);
    void showCubePieces();
    void showCubelets();
    void syncToCubelets();
    void setPieces(std::vector<Piece> PIECES);

    const int getN() const;
    const CubeState& getState() const;
    const std::vector<Cubelet>& getCubelets() const;
};