#pragma once
#include <glm/glm.hpp>
#include "src/color/color.h"

struct Cubelet {
    glm::ivec3 gridPos;
    glm::vec4 color[6];
    glm::mat4 transform;
};