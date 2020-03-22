cmake_minimum_required(VERSION 3.6)

project(gl-2-triangle)

set(CMAKE_CXX_STANDARD 11)
 
set(SOURCE_FILES
    main.cpp
    shader.cpp    
)

add_executable(gl-2-triangle ${SOURCE_FILES})
target_link_libraries(gl-2-triangle
    GL
    GLU
    glut
    GLEW
)



