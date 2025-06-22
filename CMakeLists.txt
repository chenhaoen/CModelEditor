cmake_minimum_required(VERSION 3.22)
project(CModelEditor)

function(add_cmakelists name)
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/scripts/${name}CMakeLists.txt"
    "${CMAKE_CURRENT_SOURCE_DIR}/${name}/CMakeLists.txt"
    COPYONLY
  )
endfunction()

add_cmakelists(bimg)
add_cmakelists(bx)
add_cmakelists(bgfx)

add_subdirectory(glfw)
add_subdirectory(bx)
add_subdirectory(bimg)
add_subdirectory(bgfx)
add_subdirectory(GLFWExample)
add_subdirectory(QTExample)
