cmake_minimum_required(VERSION 3.10.0)
project(computer_graphics VERSION 0.1.0 LANGUAGES C CXX)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)

add_executable(computer_graphics 
    src/main.cpp    
    src/glad.h
    src/glad.c 
    
    )
target_link_libraries(computer_graphics 
    glfw
    OpenGL::GL)

