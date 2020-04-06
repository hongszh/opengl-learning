cmake_minimum_required(VERSION 3.6)

project(gl-triangle)

SET(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_STANDARD 11)
 
set(SOURCE_FILES
    main.cpp
    shader.cpp    
)

include_directories(/usr/local/include/)
link_directories(/usr/local/lib/)

add_executable(gl-triangle ${SOURCE_FILES})
target_link_libraries(gl-triangle
    GL
    GLEW
    glfw3
    pthread
    dl
)



