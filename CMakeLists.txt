cmake_minimum_required(VERSION 3.22)
project(CModelEditor)

function(add_cmakelists name)
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/scripts/${name}CMakeLists.txt"
    "${CMAKE_CURRENT_SOURCE_DIR}/3rd/${name}/CMakeLists.txt"
    COPYONLY
  )
endfunction()

add_cmakelists(bimg)
add_cmakelists(bx)
add_cmakelists(bgfx)

add_subdirectory(3rd/glfw)
add_subdirectory(3rd/bx)
add_subdirectory(3rd/bimg)
add_subdirectory(3rd/bgfx)
add_subdirectory(GLFWExample)
add_subdirectory(QTExample)
