#pragma once
#include <string>

struct Piece {
    int id;
    std::string type;
    int gridPos[3];
    int ori;

    Piece() = default;

    Piece(int id, const std::string& type, int x, int y, int z, int ori = 0);
};