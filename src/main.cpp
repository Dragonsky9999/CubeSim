#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "src/Cube/model/cube.h"
#include "src/renderer/renderer.h"
#include <iostream>

int main() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Cube", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    Cube cube(5); 
    int N = cube.getN();
    std::vector<std::string> moves;

    moves = { "F", "U'", }; //"B", "L", "U'", "F2", "U2", "F", "U", "F'", "U2", "D'", "B", "D", "L2", "B2", "U" }; // "Fw2", "Uw'", "Bw", "Lw", "Uw'", "Fw2", "Uw2", "Fw'", "Uw2", "Dw'", "Bw", "Dw", "Lw2", "Bw2", "Uw" };
    cube.moves(moves);

    initRenderer(window);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderCubelets(cube.getCubelets(), cube.getN());

        glfwSwapBuffers(window);
    }

    shutdownRenderer();
    glfwTerminate();
    return 0;
}