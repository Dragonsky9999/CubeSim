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

    Cube cube(15);
    cube.move("F 3Rw' 7U2 12Lw 5B' 9Dw2 R 14Fw' 2U 11Rw2 6D' 4Lw F2 13Bw 8Rw' 10U' 15Dw2 3F 7L' 2Bw2 12Rw 5U2 9F' 14D 6Lw2 11Fw' B 4Uw 8R2 13D' 10Bw' 3L2 15Fw 7D2 2R' 12Uw' 5B2 9L 14Rw2 6F' 11Dw 4B' 8U2 13R 10Fw' 15L2 3Dw' 7B 2Uw2 2Uw2' 7B' 3Dw 15L2' 10Fw 13R' 8U2' 4B 11Dw' 6F 14Rw2' 9L' 5B2' 12Uw 2R 7D2' 15Fw' 3L2' 10Bw 13D 8R2' 4Uw' B' 11Fw 6Lw2' 14D' 9F 5U2' 12Rw' 2Bw2' 7L 3F' 15Dw2' 10U 8Rw 13Bw' F2' 4Lw' 6D 11Rw2' 2U' 14Fw R' 9Dw2' 5B 12Lw' 7U2' 3Rw F'");
    
    cube.syncToCubelets();
    
    initRenderer(window);

    double lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        double now = glfwGetTime();
        float  dt = float(now - lastTime);
        lastTime = now;

        glfwPollEvents();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderCubelets(cube.getCubelets(), cube.getN(), cube.getRotation());
        cube.update(dt);

        glfwSwapBuffers(window);
    }

    shutdownRenderer();
    glfwTerminate();
    return 0;
}