#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "src/Cube/model/cube.h"
#include "src/renderer/renderer.h"
#include <iostream>

#include <filesystem>
int main() {

    std::cout << std::filesystem::current_path() << std::endl;

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Cube", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    Cube cube(5); 
    
    cube.moves("F U' B L U' F2 U2 F U F' U2 D' B D L2 B2 U 2Fw 2Uw' 2Bw 2Lw 2Uw' 2Fw2 2Uw2 2Fw 2Uw 2Fw' 2Uw2 2Dw' 2Bw 2Dw 2Lw2 2Bw2 2Uw");
    
    cube.syncToCubelets();


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