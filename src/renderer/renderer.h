#pragma once

#include <vector>
#include <optional>
#include <GLFW/glfw3.h>
#include "src/Cube/transform/cubelet.h"
#include "src/Cube/Rotation/rotation.h"

using std::optional, std::vector;

void initRenderer(GLFWwindow* window);
void renderCubelets(const std::vector<Cubelet>& cubelets, const int N, const Rotation* rotation);
void shutdownRenderer();