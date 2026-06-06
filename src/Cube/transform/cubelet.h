#pragma once
#include <glm/glm.hpp>
#include "src/color/color.h"

struct Cubelet {
	glm::vec3 pos;
	glm::vec4 color[6] = { toRGBA(COLOR::Black), toRGBA(COLOR::Black), toRGBA(COLOR::Black), toRGBA(COLOR::Black), toRGBA(COLOR::Black), toRGBA(COLOR::Black) };
};	