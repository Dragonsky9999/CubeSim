#pragma once

#include <vector>
#include <string>
#include <optional>
#include <queue>
#include "src/Cube/core/cubeState.h"
#include "src/Cube/transform/cubelet.h"
#include "src/Color/color.h"
#include "src/Axis/axis.h"
#include "src/Cube/face/face.h"
#include "src/Cube/Move/move.h"
#include "src/Cube/Rotation/rotation.h"

using std::vector, std::string, std::optional, std::queue;

class Cube {
private:
    COLOR FACE_COLORS[6];
    CubeState STATE;
    vector<Cubelet> CUBELETS;

    optional<Rotation> ROTATION;
    queue<Move> MOVE_QUEUE;

    void executeMove(const Move& move);
    void rotateLayer(Axis axis, int layer, int dir);
public:
    Cube(int n = 3);

    // state access
    int getN() const;
    const CubeState& getState() const;
    const vector<Cubelet>& getCubelets() const;
    const optional<Rotation>& getRotation() const;

    // debug
    void showCubePieces() const;
    void showCubelets() const;

    // mutation
    void setPieces(const vector<Piece>& PIECES);    

    // input system
    Cube& operator<<(const string& moves);
    void move(const string& moveStrs);

    // sync layer
    void syncToCubelets();

    // update
    void update(float dt);
};