cmake_minimum_required(VERSION 3.10)
project(3DEngine)

set(CMAKE_CXX_STANDARD 17)
include_directories(include)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.c")

add_executable(3DEngine
    ${SOURCES}
)

target_link_libraries(3DEngine
    glfw
    OpenGL::GL
    cjson
)
