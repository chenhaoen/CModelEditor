cmake_minimum_required(VERSION 3.22)
project(GLFWExample)

add_executable(${PROJECT_NAME} main.cpp logo.h)

target_link_libraries(${PROJECT_NAME}
glfw
)