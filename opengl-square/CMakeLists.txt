cmake_minimum_required(VERSION 3.6)

project(gl-square)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
    main.cpp
)

add_executable(gl-square ${SOURCE_FILES})
target_link_libraries(gl-square
    GL
    GLU
    glut
    GLEW
)



