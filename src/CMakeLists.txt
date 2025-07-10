project(Core)

add_library(${PROJECT_NAME} SHARED
  ${CMAKE_SOURCE_DIR}/include/Exports.h
  ${CMAKE_SOURCE_DIR}/include/Scene/Scene.h
  ${CMAKE_SOURCE_DIR}/include/Scene/Camera.h
  ${CMAKE_SOURCE_DIR}/include/pch.h
  ${CMAKE_SOURCE_DIR}/include/FileSystem.h
  ${CMAKE_SOURCE_DIR}/src/Scene/Scene.cpp
  ${CMAKE_SOURCE_DIR}/src/Scene/Camera.cpp
  ${CMAKE_SOURCE_DIR}/src/FileSystem.cpp
)

if(MSVC)
    target_precompile_headers(Core PRIVATE "${CMAKE_SOURCE_DIR}/include/pch.h")
else()
    target_precompile_headers(Core PRIVATE
        "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_SOURCE_DIR}/include/pch.h>"
    )
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE
  CORE_EXPORT
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    Diligent-Common
)

add_subdirectory(QT)
add_subdirectory(GLFW)