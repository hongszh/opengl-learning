cmake_minimum_required(VERSION 3.6)

project(gl-loadimage)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
    main.cpp
    shader.cpp
)


add_executable(gl-draw ${SOURCE_FILES})
target_link_libraries(gl-draw
    GL
    GLU
    GLEW
    glfw3
    pthread
    dl
)



