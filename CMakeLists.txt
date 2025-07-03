cmake_minimum_required(VERSION 3.22)
project(CModelEditor)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(3rd/glfw)
add_subdirectory(3rd/DiligentCore)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(include)

add_subdirectory(src)

