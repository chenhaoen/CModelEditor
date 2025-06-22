cmake_minimum_required(VERSION 3.22)
project(GLFWExample)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

add_executable(${PROJECT_NAME} main.cpp logo.h)

target_link_libraries(${PROJECT_NAME} 
bx
glfw
bgfx
)