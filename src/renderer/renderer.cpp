#define GLFW_INCLUDE_NONE
#include "renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

// =======================================================
// Vertex
// =======================================================
struct Vertex {
    glm::vec3 pos;
    glm::vec4 color;
};

// =======================================================
// OpenGL state
// =======================================================
static GLuint vao, vbo, shaderProgram;
static GLint modelLoc, viewLoc, projLoc;

// =======================================================
// window
// =======================================================
static GLFWwindow* g_window = nullptr;

// =======================================================
// Arcball camera
// =======================================================
static glm::quat camRot = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

static float distanceCam = 12.0f;
static float targetDistance = 12.0f;

static double lastX = 0.0;
static double lastY = 0.0;
static bool firstMouse = true;

// =======================================================
// forward
// =======================================================
static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

// =======================================================
// util
// =======================================================
static std::string loadFile(const char* path) {
    std::ifstream file(path);
    if (!file) {
        std::cout << "failed : " << path << std::endl;
        std::abort();
    }
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

static GLuint compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        char log[1024];
        glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
        throw std::runtime_error(log);
    }
    return shader;
}

// =======================================================
// input
// =======================================================
static void mouseCallback(GLFWwindow*, double xpos, double ypos) {
    if (!g_window) return;

    if (glfwGetMouseButton(g_window, GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS) {
        firstMouse = true;
        return;
    }

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = float(xpos - lastX);
    float yoffset = float(ypos - lastY);

    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.005f;

    glm::vec3 worldUp(0.0f, 1.0f, 0.0f);

    glm::quat qYaw = glm::angleAxis(-xoffset * sensitivity, worldUp);
    camRot = qYaw * camRot;

    glm::vec3 right = camRot * glm::vec3(1.0f, 0.0f, 0.0f);
    glm::quat qPitch = glm::angleAxis(-yoffset * sensitivity, glm::normalize(right));
    camRot = qPitch * camRot;
}

static void mouseButtonCallback(GLFWwindow*, int button, int action, int) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        firstMouse = true;
    }
}

// =======================================================
// scroll → target distance
// =======================================================
static void scrollCallback(GLFWwindow*, double, double yoffset) {
    targetDistance -= float(yoffset) * 0.8f;
}

// =======================================================
// key
// =======================================================
static void keyCallback(GLFWwindow* window, int key, int, int action, int mods) {
    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        if (mods & GLFW_MOD_CONTROL) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    }
}

// =======================================================
// init
// =======================================================
void initRenderer(GLFWwindow* window) {
    g_window = window;

    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetKeyCallback(window, keyCallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    std::string vertSrc = loadFile("src/renderer/cube.vert");
    std::string fragSrc = loadFile("src/renderer/cube.frag");

    GLuint vert = compileShader(GL_VERTEX_SHADER, vertSrc.c_str());
    GLuint frag = compileShader(GL_FRAGMENT_SHADER, fragSrc.c_str());

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vert);
    glAttachShader(shaderProgram, frag);
    glLinkProgram(shaderProgram);

    glDeleteShader(vert);
    glDeleteShader(frag);

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    modelLoc = glGetUniformLocation(shaderProgram, "model");
    viewLoc = glGetUniformLocation(shaderProgram, "view");
    projLoc = glGetUniformLocation(shaderProgram, "projection");

    glEnable(GL_DEPTH_TEST);
}

// =======================================================
// face helper
// =======================================================
static void addFace(std::vector<Vertex>& v,
    glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3,
    glm::vec4 color)
{
    v.push_back({ p0, color });
    v.push_back({ p1, color });
    v.push_back({ p2, color });

    v.push_back({ p2, color });
    v.push_back({ p3, color });
    v.push_back({ p0, color });
}

// =======================================================
// render
// =======================================================
void renderCubelets(const std::vector<Cubelet>& cubelets, int N) {

    std::vector<Vertex> vertices;
    vertices.reserve(cubelets.size() * 36);

    // ===================================================
    // 🎯 camera distance (N adaptive)
    // ===================================================
    float halfExtent = (N - 1) * 0.5f;
    float radius = halfExtent * std::sqrt(3.0f);

    float minDist = 0.0001f;
    float maxDist = radius * 6.0f;
    float baseDist = radius * 2.8f;

    // 初期距離同期
    if (distanceCam < 0.0001f)
        distanceCam = baseDist;

    // clamp target
    targetDistance = std::clamp(targetDistance, minDist, maxDist);

    // smooth zoom
    distanceCam += (targetDistance - distanceCam) * 0.15f;

    float h = 0.475f;

    for (const Cubelet& c : cubelets) {

        glm::vec3 p = c.pos;

        glm::vec3 v000 = p + glm::vec3(-h, -h, -h);
        glm::vec3 v001 = p + glm::vec3(-h, -h, +h);
        glm::vec3 v010 = p + glm::vec3(-h, +h, -h);
        glm::vec3 v011 = p + glm::vec3(-h, +h, +h);
        glm::vec3 v100 = p + glm::vec3(+h, -h, -h);
        glm::vec3 v101 = p + glm::vec3(+h, -h, +h);
        glm::vec3 v110 = p + glm::vec3(+h, +h, -h);
        glm::vec3 v111 = p + glm::vec3(+h, +h, +h);

        addFace(vertices, v011, v111, v110, v010, c.color[0]);
        addFace(vertices, v000, v100, v101, v001, c.color[1]);
        addFace(vertices, v001, v101, v111, v011, c.color[2]);
        addFace(vertices, v110, v100, v000, v010, c.color[3]);
        addFace(vertices, v101, v100, v110, v111, c.color[4]);
        addFace(vertices, v000, v001, v011, v010, c.color[5]);
    }

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER,
        vertices.size() * sizeof(Vertex),
        vertices.data(),
        GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);

    glUseProgram(shaderProgram);

    // ===================================================
    // camera
    // ===================================================
    glm::vec3 center(0.0f);

    glm::vec3 camDir = camRot * glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 camUp = camRot * glm::vec3(0.0f, 1.0f, 0.0f);

    glm::vec3 eye = center + camDir * distanceCam;

    glm::mat4 view = glm::lookAt(eye, center, camUp);

    glm::mat4 projection =
        glm::perspective(glm::radians(45.0f),
            1280.0f / 720.0f,
            0.1f,
            100.0f);

    glm::mat4 model(1.0f);

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertices.size());
}

// =======================================================
void shutdownRenderer() {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteProgram(shaderProgram);
}