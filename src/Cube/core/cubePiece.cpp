#include "src/Cube/core/cubePiece.h"
#include <string>

#include <cmath>

#include <iostream>

static int cornerChirality(int gridPos[3], int N) {
    int bx = gridPos[0] / (N - 1);
    int by = gridPos[1] / (N - 1);
    int bz = gridPos[2] / (N - 1);
    return (bx + by + bz + 1) % 2;
}

Piece::Piece(int id, const std::string& type, int x, int y, int z, int ori)
    : id(id), type(type), ori(ori) {
    gridPos[0] = x;
    gridPos[1] = y;
    gridPos[2] = z;
}

void Piece::rotate(int N, Axis axis, int dir) {
    int& a = gridPos[((int)axis + 1) % 3];
    int& b = gridPos[((int)axis + 2) % 3];

    int newA, newB;
    if (dir == 1) {
        newA = b;
        newB = (N - 1) - a;
    }
    else {
        newA = (N - 1) - b;
        newB = a;
    }
    a = newA;
    b = newB;

    // ori の更新
    // edge
    if (type == "edge") {
        if ((axis == Axis::X && (gridPos[(int)Axis::X] != 0 && gridPos[(int)Axis::X] != N - 1)) || (axis == Axis::Y && (gridPos[(int)Axis::Y] != 0 && gridPos[(int)Axis::Y] != N - 1)) || axis == Axis::Z) {
            ori = (ori + 1) % 2;
        }
    }

    // corner
    if (type == "corner") {
    int chirality = cornerChirality(gridPos, N);
    
        if (axis == Axis::X) {
            ori = (ori + (chirality == 0 ? 1 : 2)) % 3;
        }else if (axis == Axis::Z) {
            ori = (ori + (chirality == 0 ? 2 : 1)) % 3;
        }
    }
}