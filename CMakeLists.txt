cmake_minimum_required( VERSION 3.24 )
project( nxgl )

set( CMAKE_CXX_STANDARD 20 )

set( VENDOR_INCL D:/projects )
set( VCPKG_INCL D:/vcpkg/installed/x64-windows-static-md/include )

add_executable( ${PROJECT_NAME}
  main.cpp
  D:/projects/glad/v3.3/src/gl.c
  D:/projects/imgui/imgui.cpp
  D:/projects/imgui/imgui_draw.cpp
  D:/projects/imgui/imgui_tables.cpp
  D:/projects/imgui/imgui_widgets.cpp
  D:/projects/imgui/backends/imgui_impl_glfw.cpp
  D:/projects/imgui/backends/imgui_impl_opengl3.cpp
  )

#####################################################################################
#
# RESOURCES
#
#####################################################################################
# https://www.youtube.com/watch?v=sTuIRzrB2dA
add_custom_target( copy_resources ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${PROJECT_SOURCE_DIR}/resources
  ${PROJECT_BINARY_DIR}/resources
  COMMENT "copying resources to binary directory"
  )

add_dependencies( ${PROJECT_NAME} copy_resources )

#####################################################################################
#
# LIBRARY DEPENDENCIES
#
#####################################################################################
set( BUILD_DEPENDS_LIBS
  # libs that are required and there is no debug/release qualifier
  opengl32.lib
  )

if (CMAKE_BUILD_TYPE STREQUAL "Debug" )
  set( DEBUG_LIB_DIR D:/vcpkg/installed/x64-windows-static-md/debug/lib )

  list( APPEND BUILD_DEPENDS_LIBS
    ${DEBUG_LIB_DIR}/glfw3.lib
    )
else()
  set( RELEASE_LIB_DIR D:/vcpkg/installed/x64-windows-static-md/lib )

  list( APPEND BUILD_DEPENDS_LIBS
    ${RELEASE_LIB_DIR}/glfw3.lib
    )
endif()

#####################################################################################
#
# INCLUDE DIRECTORIES
#
#####################################################################################
target_include_directories(
  ${PROJECT_NAME}
  PRIVATE
  ${VCPKG_INCL}
  ${VENDOR_INCL}/glad/v3.3/include
  ${VENDOR_INCL}/imgui/backends
  ${VENDOR_INCL}/bitsery/include
  ${VENDOR_INCL}/spdlog/include
  ${VENDOR_INCL}/imgui
  ${VENDOR_INCL}/json/include
  ${VENDOR_INCL}/entt
  ${VENDOR_INCL}/glm
  ${CMAKE_SOURCE_DIR}
)

#####################################################################################
#
# PRECOMPILED HEADERS
#
#####################################################################################
target_precompile_headers( ${PROJECT_NAME}
  PUBLIC
# adding gl.c to the executable and using a precompiled header
# causes funky c++ std errors
#  Pch.hpp
  )

target_link_libraries( ${PROJECT_NAME}
  PRIVATE
  ${BUILD_DEPENDS_LIBS}
  )

#add_definitions( -DNOMINMAX )