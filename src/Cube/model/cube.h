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

    // get original state
    int getN() const;
    const CubeState& getState() const;
    const std::vector<Cubelet>& getCubelets() const;

    // show state on terminal
    void showCubePieces() const;
    void showCubelets() const;

    // change state
    void setPieces(const std::vector<Piece>& PIECES);    
    Cube& operator<<(const std::string& moves);
    void move(const std::string& moveStrs);
    void syncToCubelets();
};