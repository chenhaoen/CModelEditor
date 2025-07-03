cmake_minimum_required(VERSION 3.22)
project(QTExample)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

qt_standard_project_setup()

add_executable(${PROJECT_NAME} 
${CMAKE_SOURCE_DIR}/include/QT/MainWindow.h
${CMAKE_SOURCE_DIR}/include/QT/SceneWindow.h
main.cpp 
MainWindow.cpp
MainWindow.ui
SceneWindow.cpp
)

target_compile_definitions(${PROJECT_NAME} PRIVATE ENGINE_DLL=1)

target_link_libraries(${PROJECT_NAME} 
${Qt6Core_LIBRARIES}
${Qt6Gui_LIBRARIES}
${Qt6Widgets_LIBRARIES}
Qt6::GuiPrivate
Diligent-GraphicsEngineVk-shared
Diligent-Common
Diligent-GraphicsTools
Diligent-BuildSettings
Core
)

if(MSVC)
set(OUTPUT_DIR  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE})
else()
set(OUTPUT_DIR  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND MSVC)
    set(QT_SUFFIX "d")
else()
    set(QT_SUFFIX "")
endif()

if(WIN32)
    copy_required_dlls(${PROJECT_NAME})
    
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${Qt6_DIR}/../../../plugins/styles
            ${OUTPUT_DIR}/styles
    
        COMMAND ${CMAKE_COMMAND} -E copy_directory
           ${Qt6_DIR}/../../../plugins/platforms
           ${OUTPUT_DIR}/platforms
    
        COMMAND ${CMAKE_COMMAND} -E copy
           ${Qt6_DIR}/../../../bin/Qt6Gui${QT_SUFFIX}.dll
           ${OUTPUT_DIR}/Qt6Gui${QT_SUFFIX}.dll
    
        COMMAND ${CMAKE_COMMAND} -E copy
           ${Qt6_DIR}/../../../bin/Qt6Core${QT_SUFFIX}.dll
           ${OUTPUT_DIR}/Qt6Core${QT_SUFFIX}.dll
    
        COMMAND ${CMAKE_COMMAND} -E copy
           ${Qt6_DIR}/../../../bin/Qt6Widgets${QT_SUFFIX}.dll
           ${OUTPUT_DIR}/Qt6Widgets${QT_SUFFIX}.dll
    )
endif()

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources
        ${OUTPUT_DIR}/resources
)
