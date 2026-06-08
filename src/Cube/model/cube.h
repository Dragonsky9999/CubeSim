#pragma once

#include <vector>
#include <string>
#include "src/Cube/core/cubeState.h"
#include "src/Cube/transform/cubelet.h"
#include "src/Color/color.h"
#include "src/Axis/axis.h"
#include "src/Cube/face/face.h"
#include "src/Cube/Move/move.h"

class Cube {
private:
    COLOR FACE_COLORS[6];
    CubeState STATE;
    std::vector<Cubelet> CUBELETS;

    void executeMove(const Move& move);
    void rotateLayer(Axis axis, int layer, int dir);
public:
    Cube(int n = 3);

    // state access
    int getN() const;
    const CubeState& getState() const;
    const std::vector<Cubelet>& getCubelets() const;

    // debug
    void showCubePieces() const;
    void showCubelets() const;

    // mutation
    void setPieces(const std::vector<Piece>& PIECES);    

    // input system
    Cube& operator<<(const std::string& moves);
    void move(const std::string& moveStrs);

    // sync layer
    void syncToCubelets();
};