#include "src/Cube/cubePiece.h"
#include <string>

Piece::Piece(int id, const std::string& type, int x, int y, int z, int ori)
    : id(id), type(type), ori(ori) {
    gridPos[0] = x;
    gridPos[1] = y;
    gridPos[2] = z;
}