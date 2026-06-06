#include "src/Cube/cubeState.h"

#include <string>

CubeState::CubeState(unsigned int n)
    : N(n){
    pieces.resize(N * N * N);
}

void CubeState::initCube() { 
    for (int x = 0; x < (int)N; x++) {
        for (int y = 0; y < (int)N; y++) {
            for (int z = 0; z < (int)N; z++) {

                int id = x + y * N + z * N * N;

                int faceCount = 0;
                if (x == 0 || x == N - 1) faceCount++;
                if (y == 0 || y == N - 1) faceCount++;
                if (z == 0 || z == N - 1) faceCount++;

                std::string type;
                if (faceCount == 3) type = "corner";
                else if (faceCount == 2) type = "edge";
                else type = "center";

                pieces[id] = Piece(id, type, x, y, z, 0);
            }
        }
    }
}

void CubeState::reset() {
    initCube();
}