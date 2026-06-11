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

    Cube cube(30); 
    cube.move("12F 7U2' 19R 3B' 30U2 14L' 8D2' 25F' 2R2' 11U 27B 5L2 16U' 9F2' 21R' 30D 18B2' 6F 13L 24U2' 1R 10B' 23F2 29L' 4U2' 15R2 17D' 28F 20B2 22L' 3U 7F' 12R2' 26B 9L2' 30F' 5U2 14R' 18D2 11F' 2L 6U' 19B2 21F' 8R2 27L' 16D2' 24B' 13F2' 10U2 25R' 1B2' 7L 29F 15U' 3R2 22B' 17F2' 28D 4L' 9U 20R2' 12B' 6F2' 23L 30U' 14B2 5F' 18R 11D2' 2F 16L' 27U2' 8B 19R2 24F' 1U2' 13B 21L2' 26R' 10F' 7R2 29B' 3L2' 15F2' 22U 28R2' 4B' 17L2 6D' 25U' 12F2 9R' 30B2' 18F' 23R 11L2' 14U2' 20F 16B2')");
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

        renderCubelets(cube.getCubelets(), cube.getN());
        cube.update(dt);

        glfwSwapBuffers(window);
    }

    shutdownRenderer();
    glfwTerminate();
    return 0;
}