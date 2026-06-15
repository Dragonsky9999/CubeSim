#define GLM_ENABLE_EXPERIMENTAL
#include "src/Cube/model/cube.h"

#include <iostream>
#include <string>
#include <cctype>
#include <sstream>
#include <stdexcept>

#include <glm/gtx/rotate_vector.hpp>

float timer = 0.0f;
float duration = 0.0001f;

using std::cout, std::endl;

// init
Cube::Cube(int n): STATE(n) {
        STATE.initCube();
		FACE_COLORS[0] = COLOR::White;
		FACE_COLORS[1] = COLOR::Yellow;
		FACE_COLORS[2] = COLOR::Green;
		FACE_COLORS[3] = COLOR::Blue;
		FACE_COLORS[4] = COLOR::Red;
		FACE_COLORS[5] = COLOR::Orange;
}

static int normal_to_face(const glm::vec3& n) {
    if (n.x > 0.5f) return getIndex(Face::R);
    if (n.x < -0.5f) return getIndex(Face::L);
    if (n.y > 0.5f) return getIndex(Face::U);
    if (n.y < -0.5f) return getIndex(Face::D);
    if (n.z > 0.5f) return getIndex(Face::F);
    if (n.z < -0.5f) return getIndex(Face::B);
    return -1;
}

// ==================
// get original State
// ==================

int Cube::getN() const{
    return STATE.N;
}
const CubeState& Cube::getState() const{
    return STATE;
}
const std::vector<Cubelet>& Cube::getCubelets() const{
    return CUBELETS;
}
const optional<Rotation>* Cube::getRotation() const {
    return &ROTATION;
}
// ==================

// ======================
// show state on terminal
// ======================
static std::string getPiecesInfo(const std::vector<Piece>& pieces) {
    std::string s;
    
    for (const auto& piece : pieces) {
        s += "Piece("
                + std::to_string(piece.id) + "," + '"' + 
                piece.type + '"' + ", " +
                std::to_string(piece.gridPos[0]) + ", " +
                std::to_string(piece.gridPos[1]) + ", " +
                std::to_string(piece.gridPos[2]) + ", " +
                std::to_string(piece.ori) + "),\n";
    }

    return s;
}

static std::string getCubeletsInfo(const std::vector<Cubelet>& cubelets) {
    std::string s;

    for (const auto& cubelet : cubelets) {
        s += "pos: (" +
			    std::to_string(cubelet.gridPos[0]) + ", " + 
			    std::to_string(cubelet.gridPos[1]) + ", " + 
			    std::to_string(cubelet.gridPos[2]) + 
            ")" + "," +
            "colors: [" +
			    "rgb(" + std::to_string(cubelet.color[0][0]) + ", " + std::to_string(cubelet.color[0][1]) + ", " + std::to_string(cubelet.color[0][2]) + "), " +
                "rgb(" + std::to_string(cubelet.color[1][0]) + ", " + std::to_string(cubelet.color[1][1]) + ", " + std::to_string(cubelet.color[1][2]) + "), " +
                "rgb(" + std::to_string(cubelet.color[2][0]) + ", " + std::to_string(cubelet.color[2][1]) + ", " + std::to_string(cubelet.color[2][2]) + "), " +
                "rgb(" + std::to_string(cubelet.color[3][0]) + ", " + std::to_string(cubelet.color[3][1]) + ", " + std::to_string(cubelet.color[3][2]) + "), " +
                "rgb(" + std::to_string(cubelet.color[4][0]) + ", " + std::to_string(cubelet.color[4][1]) + ", " + std::to_string(cubelet.color[4][2]) + "), " +
                "rgb(" + std::to_string(cubelet.color[5][0]) + ", " + std::to_string(cubelet.color[5][1]) + ", " + std::to_string(cubelet.color[5][2]) + ")" +
			"]\n";    
    }
    return s;
}


void Cube::showCubePieces() const{
    std::string s =
        "CubePieces(N=" + std::to_string(STATE.N) +
        ", count=" + std::to_string(STATE.pieces.size()) +
        ", pieces=[\n" + getPiecesInfo(STATE.pieces) + "]\n" + 
        ")";
    std::cout << s << std::endl;
}

void Cube::showCubelets() const{
    std::string s =
        "CubeLets(N=" + std::to_string(STATE.N) +
        ", count=" + std::to_string(CUBELETS.size()) +
        ", pieces=[\n" + getCubeletsInfo(CUBELETS) + "]";
    std::cout << s << std::endl;
}
// ===========================


// ===========================
// change state
// ===========================
Cube& Cube::operator<<(const string& moves)
{
    move(moves);
    return *this;
}

void Cube::setPieces(const std::vector<Piece>& PIECES) {
    STATE.pieces = PIECES;
}

void Cube::syncToCubelets() {
    CUBELETS.clear();
    for (const auto& piece : STATE.pieces) {

        // Skip inner pieces (not visible)
        if (piece.gridPos[0] != 0 && piece.gridPos[0] != STATE.N - 1 && piece.gridPos[1] != 0 && piece.gridPos[1] != STATE.N - 1 &&piece.gridPos[2] != 0 && piece.gridPos[2] != STATE.N - 1) continue;

        // Set position based on piece pos
        Cubelet cubelet;
        cubelet.gridPos = glm::ivec3(piece.gridPos[0], piece.gridPos[1], piece.gridPos[2]);
        
        // Set colors based on piece type and pos/ori
        int orig_x = piece.id % STATE.N; int orig_y = (piece.id / STATE.N) % STATE.N; int orig_z = (piece.id / STATE.N / STATE.N);

        std::vector<glm::vec3> orig_normals;
        std::vector<glm::vec4> orig_colors;

        std::vector<glm::vec3> curr_normals;


        if (orig_x == 0)            { orig_normals.push_back({ -1, 0, 0 }); orig_colors.push_back(toRGBA(FACE_COLORS[(int)Face::L])); }
        if (orig_x == STATE.N - 1)  { orig_normals.push_back({ 1, 0, 0 });  orig_colors.push_back(toRGBA(FACE_COLORS[(int)Face::R])); }
        if (orig_y == 0)            { orig_normals.push_back({ 0, -1, 0 }); orig_colors.push_back(toRGBA(FACE_COLORS[(int)Face::D])); }
        if (orig_y == STATE.N - 1)  { orig_normals.push_back({ 0, 1, 0 });  orig_colors.push_back(toRGBA(FACE_COLORS[(int)Face::U])); }
        if (orig_z == 0)            { orig_normals.push_back({ 0, 0, -1 }); orig_colors.push_back(toRGBA(FACE_COLORS[(int)Face::B])); }
        if (orig_z == STATE.N - 1)  { orig_normals.push_back({ 0, 0, 1 });  orig_colors.push_back(toRGBA(FACE_COLORS[(int)Face::F])); }

        if (piece.gridPos[0] == 0)           curr_normals.push_back({ -1,  0,  0 });
        if (piece.gridPos[0] == STATE.N - 1) curr_normals.push_back({ 1,  0,  0 });
        if (piece.gridPos[1] == 0)           curr_normals.push_back({ 0,  -1,  0 });
        if (piece.gridPos[1] == STATE.N - 1) curr_normals.push_back({ 0,  1,  0 });
        if (piece.gridPos[2] == 0)           curr_normals.push_back({ 0,  0,  -1 });
        if (piece.gridPos[2] == STATE.N - 1) curr_normals.push_back({ 0,  0,  1 });

        if (curr_normals.size() == 1) {

            cubelet.color[normal_to_face(curr_normals[0])] = orig_colors[0];
        }else if (curr_normals.size() == 2) {

            bool has_y = (curr_normals[0].y != 0 || curr_normals[1].y != 0);

            int orig_ref_idx = (orig_normals[0].y != 0) ? 0 : ((orig_normals[1].y != 0) ? 1 : ((orig_normals[0].z != 0) ? 0 : 1));
            int curr_ref_idx = has_y ? ((curr_normals[0].y != 0) ? 0 : 1) : ((curr_normals[0].z != 0) ? 0 : 1);

            int orig_other_idx = 1 - orig_ref_idx;
            int curr_other_idx = 1 - curr_ref_idx;

            if (piece.ori == 0) {
                cubelet.color[normal_to_face(curr_normals[curr_ref_idx])] = orig_colors[orig_ref_idx];
                cubelet.color[normal_to_face(curr_normals[curr_other_idx])] = orig_colors[orig_other_idx];
            }
            else {
                cubelet.color[normal_to_face(curr_normals[curr_ref_idx])] = orig_colors[orig_other_idx];
                cubelet.color[normal_to_face(curr_normals[curr_other_idx])] = orig_colors[orig_ref_idx];
            }
        }else if (curr_normals.size() == 3) {
            
                int orig_y_idx = (orig_normals[0].y != 0) ? 0 : ((orig_normals[1].y != 0) ? 1 : 2);
                int curr_y_idx = (curr_normals[0].y != 0) ? 0 : ((curr_normals[1].y != 0) ? 1 : 2);
                
                int o_a = (orig_y_idx + 1) % 3, o_b = (orig_y_idx + 2) % 3;
                if (glm::dot(glm::cross(orig_normals[orig_y_idx], orig_normals[o_a]), orig_normals[o_b]) < 0) std::swap(o_a, o_b);

                int c_a = (curr_y_idx + 1) % 3, c_b = (curr_y_idx + 2) % 3;
                if (glm::dot(glm::cross(curr_normals[curr_y_idx], curr_normals[c_a]), curr_normals[c_b]) < 0) std::swap(c_a, c_b);

                if (piece.ori == 0) {
                    cubelet.color[normal_to_face(curr_normals[curr_y_idx])] = orig_colors[orig_y_idx];
                    cubelet.color[normal_to_face(curr_normals[c_a])] = orig_colors[o_a];
                    cubelet.color[normal_to_face(curr_normals[c_b])] = orig_colors[o_b];
                }
                else if (piece.ori == 1) {
                    cubelet.color[normal_to_face(curr_normals[c_a])] = orig_colors[orig_y_idx];
                    cubelet.color[normal_to_face(curr_normals[c_b])] = orig_colors[o_a];
                    cubelet.color[normal_to_face(curr_normals[curr_y_idx])] = orig_colors[o_b];
                }
                else if (piece.ori == 2) {
                    cubelet.color[normal_to_face(curr_normals[c_b])] = orig_colors[orig_y_idx];
                    cubelet.color[normal_to_face(curr_normals[curr_y_idx])] = orig_colors[o_a];
                    cubelet.color[normal_to_face(curr_normals[c_a])] = orig_colors[o_b];
                }


        }else {
            std::cout << "YOU CANNOT CHANGE ORIENTATION FOR THIS CUBE" << std::endl;
            for (int i = 0; i < orig_colors.size(); i++) {
                std::cout << orig_colors[i].x << std::endl;
                cubelet.color[i] = orig_colors[i];
            }
        }
        
        CUBELETS.push_back(cubelet);
    }
}

void Cube::rotateLayer(Axis axis, int layer, int dir) {
    for (Piece& piece : STATE.pieces) {
        if (piece.gridPos[(int)axis] != layer) continue;

        piece.rotate(STATE.N, axis, dir);
    }
}

void Cube::executeMove(const Move& move) {
    int minLayer = std::min(move.startLayer, move.endLayer);
    int maxLayer = std::max(move.startLayer, move.endLayer);

    for (int layer = minLayer; layer <= maxLayer; layer++) {
        for (int i = 0; i < abs(move.amount); i++) {
            rotateLayer(move.axis, layer, move.dir);
        }
    }
}

void Cube::move(const std::string& moveStrs) {
    std::stringstream ss(moveStrs);
    std::string moveStr;

    while (ss >> moveStr) {
        Move move;
        int N = STATE.N;

        int depth = 1;
        size_t pos = 0;

        //--------------------------------------------------
        // width
        //--------------------------------------------------

        if (std::isdigit(moveStr[0])) {
            while (pos < moveStr.size() && std::isdigit(moveStr[pos])) pos++;

            depth = std::stoi(moveStr.substr(0, pos));
        }

        //--------------------------------------------------
        // face
        //--------------------------------------------------

        char c = moveStr[pos++];
        bool lower = std::islower(c);
        c = std::toupper(c);

        //--------------------------------------------------
        // wide
        //--------------------------------------------------

        bool wide = false;

        if (pos < moveStr.size() && moveStr[pos] == 'w') {
            wide = true;
            pos++;
        }

        //--------------------------------------------------
        // amount/dir
        //--------------------------------------------------

        int amount = 1;
        int dir = 1;

        size_t amountStartPos = pos;

        if (std::isdigit(moveStr[pos])) {
            while (pos < moveStr.size() && std::isdigit(moveStr[pos])) pos++;
            amount = std::stoi(moveStr.substr(amountStartPos, pos - amountStartPos));
        }

        if (pos < moveStr.size() && moveStr[pos] == '\'') {
            dir = -1;
        }

        //--------------------------------------------------
        // build Move
        //--------------------------------------------------

        switch (c) {
        case 'R':
            move = { Axis::X, N - depth, (wide == true) ? N - 1 : N - depth, amount, dir };
            break;

        case 'L':
            move = { Axis::X, depth - 1, (wide == true) ? 0 : depth - 1, amount, -dir };
            break;

        case 'U':
            move = { Axis::Y, N - depth, (wide == true) ? N - 1 : N - depth, amount, dir };
            break;

        case 'D':
            move = { Axis::Y, depth - 1, (wide == true) ? 0 : depth - 1, amount, -dir };
            break;

        case 'F':
            move = { Axis::Z, N - depth, (wide == true) ? N - 1 : N - depth, amount, dir };
            break;

        case 'B':
            move = { Axis::Z, depth - 1, (wide == true) ? 0 : depth - 1, amount, -dir };
            break;
            // special move (M, E, S)
        case 'M':
            move = { Axis::X, 1, N - 2, amount, -dir };
            break;
        case 'E':
            move = { Axis::Y, 1, N - 2, amount, -dir };
            break;
        case 'S':
            move = { Axis::Z, 1, N - 2, amount, dir };
            break;

        default:
            throw std::runtime_error("Invalid move");
        }

        MOVE_QUEUE.push(move);
    }
}
// ===========================


// update
void Cube::update(float dt) {
    if (!ROTATION) {

        if (timer < duration) {
            timer += dt;
            return;
        }

        if (!MOVE_QUEUE.empty()) {
            ROTATION.emplace(MOVE_QUEUE.front());
            MOVE_QUEUE.pop();
        }
        else {
            return;
        }
    }

    ROTATION->update(dt);
    timer += dt;

    if (ROTATION->finished()) {
        executeMove(ROTATION->move);
        syncToCubelets();
        ROTATION.reset();
        timer = 0.0f;
    }
}