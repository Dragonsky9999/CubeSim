cmake_minimum_required(VERSION 3.20)

project(CubeSim)

set(CMAKE_CXX_STANDARD 20)

file(COPY ${CMAKE_SOURCE_DIR}/src/renderer
     DESTINATION ${CMAKE_BINARY_DIR}/src)

find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_executable(CubeSim
    src/main.cpp
    src/renderer/renderer.cpp
    src/Cube/cube.cpp
    src/Cube/cubeState.cpp
    src/Cube/cubePiece.cpp
    src/color/color.cpp
    src/Cube/face/face.cpp
)

target_link_libraries(CubeSim PRIVATE
    glad::glad
    glm::glm
    glfw
    opengl32
)

target_include_directories(CubeSim PRIVATE
    ${CMAKE_SOURCE_DIR}
)