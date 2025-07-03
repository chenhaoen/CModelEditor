cmake_minimum_required(VERSION 3.22)
project(QTExample)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

qt_standard_project_setup()

add_executable(${PROJECT_NAME} 
main.cpp 
MainWindow.h
MainWindow.cpp
MainWindow.ui
SceneWindow.h
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

if(WIN32)
copy_required_dlls(${PROJECT_NAME})
endif()

if(MSVC)
set(BIN_DIR  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE})
else()
set(BIN_DIR  ${CMAKE_CURRENT_BINARY_DIR})
endif()

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources
        ${BIN_DIR}/resources
)

install(CODE "
    include(BundleUtilities)
    
    set(executable \"\${CMAKE_INSTALL_PREFIX}/bin/${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX}\")
    
    fixup_bundle(
        \"\${executable}\"
        \"\"  
        \"${QT6_INSTALL_PREFIX}/bin;\"
    )
")
