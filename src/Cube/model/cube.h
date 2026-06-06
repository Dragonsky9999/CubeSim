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

    void applyMove(const Move& move);
public:
    Cube(int n = 3);

    // get original state
    const int getN() const;
    const CubeState& getState() const;
    const std::vector<Cubelet>& getCubelets() const;

    // show state on terminal
    void showCubePieces();
    void showCubelets();

    // change state
    void setPieces(std::vector<Piece> PIECES);
    void syncToCubelets();
    void rotate(Axis axis, int layer, int dir);
    void move(std::string moveStr);
    void moves(std::vector<std::string>& moveStrs);
};