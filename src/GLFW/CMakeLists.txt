cmake_minimum_required(VERSION 3.22)
project(GLFWExample)

add_executable(${PROJECT_NAME} main.cpp)

target_compile_options(${PROJECT_NAME} PRIVATE -DUNICODE)

target_compile_definitions(${PROJECT_NAME} PRIVATE ENGINE_DLL=1)

target_link_libraries(${PROJECT_NAME}
glfw
#Diligent-GraphicsEngineD3D11-shared
#Diligent-GraphicsEngineOpenGL-shared
#Diligent-GraphicsEngineD3D12-shared
Diligent-GraphicsEngineVk-shared
Diligent-Common
Diligent-GraphicsTools
Diligent-BuildSettings
)

if(WIN32)
copy_required_dlls(${PROJECT_NAME})
endif()