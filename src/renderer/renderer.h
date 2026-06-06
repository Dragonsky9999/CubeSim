#pragma once

#include <vector>
#include "src/Cube/cubelet.h"
#include <GLFW/glfw3.h>

void initRenderer(GLFWwindow* window);
void renderCubelets(const std::vector<Cubelet>& cubelets, int N);
void shutdownRenderer();