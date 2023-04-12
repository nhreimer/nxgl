cmake_minimum_required( VERSION 3.24 )
project( nxgl )

set( CMAKE_CXX_STANDARD 20 )

set( VENDOR_INCL D:/projects )
set( VCPKG_INCL D:/vcpkg/installed/x64-windows-static-md/include )

add_executable( ${PROJECT_NAME}
  main.cpp
  ${VENDOR_INCL}/glad/v4.6/src/gl.c
  ${VENDOR_INCL}/imgui/imgui.cpp
  ${VENDOR_INCL}/imgui/imgui_draw.cpp
  ${VENDOR_INCL}/imgui/imgui_tables.cpp
  ${VENDOR_INCL}/imgui/imgui_widgets.cpp
  ${VENDOR_INCL}/imgui/backends/imgui_impl_glfw.cpp
  ${VENDOR_INCL}/imgui/backends/imgui_impl_opengl3.cpp

  # gfx/primitives
  ${CMAKE_SOURCE_DIR}/gfx/primitives/GLData.cpp
  ${CMAKE_SOURCE_DIR}/gfx/primitives/GLShader.cpp
  ${CMAKE_SOURCE_DIR}/gfx/primitives/GLUtility.cpp

  # gfx/shapes
  ${CMAKE_SOURCE_DIR}/gfx/shapes/NXDefaultShader.cpp
  ${CMAKE_SOURCE_DIR}/gfx/shapes/NXShape.cpp
  ${CMAKE_SOURCE_DIR}/gfx/shapes/NXSharedShape.cpp
  ${CMAKE_SOURCE_DIR}/gfx/shapes/NXSharedShapeContainer.cpp


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
  ${VENDOR_INCL}/glad/v4.6/include
  ${VENDOR_INCL}/imgui/backends
  ${VENDOR_INCL}/bitsery/include
  ${VENDOR_INCL}/spdlog/include
  ${VENDOR_INCL}/imgui
  ${VENDOR_INCL}/json/include
  ${VENDOR_INCL}/entt/src
  ${VENDOR_INCL}/glm
  ${CMAKE_SOURCE_DIR}
)

#####################################################################################
#
# PRECOMPILED HEADERS
#
#####################################################################################

target_link_libraries( ${PROJECT_NAME}
  PRIVATE
  ${BUILD_DEPENDS_LIBS}
  )

#add_definitions( -DNOMINMAX )