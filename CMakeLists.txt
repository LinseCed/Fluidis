cmake_minimum_required(VERSION 3.10)
project(3DEngine)

set(CMAKE_CXX_STANDARD 17)
include_directories(include)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(3DEngine
    src/main.c
    src/glad.c
    src/render/shader.c
    src/render/mesh.c
)

target_link_libraries(3DEngine
    glfw
    OpenGL::GL
)
