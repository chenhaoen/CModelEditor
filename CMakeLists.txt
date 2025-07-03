cmake_minimum_required(VERSION 3.22)
project(CModelEditor)

set(CMAKE_CXX_STANDARD 20)

add_library(Core SHARED
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Exports.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Scene/Scene.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Scene/Camera.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/pch.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/CFileSystem.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Scene/Scene.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Scene/Camera.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/CFileSystem.cpp
)

if(MSVC)
    # MSVC需要特殊处理
    target_precompile_headers(Core PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/pch.h")
else()
    target_precompile_headers(Core PRIVATE
        "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/include/pch.h>"
    )
endif()

target_compile_definitions(Core PRIVATE
  CORE_EXPORT
)

target_include_directories(Core PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(Core PUBLIC
    Diligent-Common
)

add_subdirectory(3rd/glfw)
add_subdirectory(3rd/DiligentCore)

add_subdirectory(GLFWExample)
add_subdirectory(QTExample)
