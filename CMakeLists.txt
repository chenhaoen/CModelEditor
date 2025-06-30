cmake_minimum_required(VERSION 3.22)
project(CModelEditor)

add_library(Core SHARED
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Exports.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Scene/Scene.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Scene/Scene.cpp
)

target_compile_definitions(Core PRIVATE
  CORE_EXPORT
)

add_subdirectory(3rd/glfw)
add_subdirectory(3rd/DiligentCore)
add_subdirectory(3rd/DiligentFX)
add_subdirectory(3rd/DiligentTools)
add_subdirectory(GLFWExample)
add_subdirectory(QTExample)
