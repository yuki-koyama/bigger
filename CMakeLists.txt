cmake_minimum_required(VERSION 3.1)

project(bigger CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(BGFX_AMALGAMATED        OFF CACHE INTERNAL "" FORCE)
set(BGFX_CONFIG_DEBUG       OFF CACHE INTERNAL "" FORCE)
set(BGFX_INSTALL            OFF CACHE INTERNAL "" FORCE)
set(BGFX_INSTALL_EXAMPLES   OFF CACHE INTERNAL "" FORCE)
set(BGFX_USE_DEBUG_SUFFIX   OFF CACHE INTERNAL "" FORCE)
set(BGFX_USE_OVR            OFF CACHE INTERNAL "" FORCE)
set(BIGG_ASSET_GEN          OFF CACHE INTERNAL "" FORCE)
set(BIGG_EXAMPLES           OFF CACHE INTERNAL "" FORCE)
set(BUILD_SHARED_LIBS       OFF CACHE INTERNAL "" FORCE)
set(BX_AMALGAMATED          OFF CACHE INTERNAL "" FORCE)
set(GLFW_DOCUMENT_INTERNALS OFF CACHE INTERNAL "" FORCE)
set(GLFW_USE_CHDIR          ON  CACHE INTERNAL "" FORCE)
set(GLFW_USE_MENUBAR        ON  CACHE INTERNAL "" FORCE)
set(GLFW_USE_RETINA         ON  CACHE INTERNAL "" FORCE)
set(GLFW_USE_OSMESA         OFF CACHE INTERNAL "" FORCE)
set(GLFW_VULKAN_STATIC      OFF CACHE INTERNAL "" FORCE)
set(LIB_SUFFIX              ""  CACHE INTERNAL "" FORCE)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/bigg)

# Note: This was necessary to avoid a run-time error on macOS/OpenGL. Without
# this option, the NSOpenGLViewContext's setView method was called from a
# wrong (?) thread during bgfx::init and then the app crashes. This needs to be
# investigated further.
target_compile_definitions(bgfx PUBLIC BGFX_CONFIG_MULTITHREADED=0)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/rand-util)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/string-util)

set(TINYOBJLOADER_BUILD_OBJ_STICHER OFF CACHE INTERNAL "" FORCE)
set(TINYOBJLOADER_BUILD_TEST_LOADER OFF CACHE INTERNAL "" FORCE)
set(TINYOBJLOADER_USE_DOUBLE        OFF CACHE INTERNAL "" FORCE)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/tinyobjloader)

set(headers
  ${CMAKE_CURRENT_SOURCE_DIR}/include/bigger/app.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/bigger/camera.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/bigger/material.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/bigger/primitive.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/bigger/scene-object.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/bigger/screen-shot-callback.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/bigger/utils.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/bigger/materials/blinnphong-material.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/bigger/primitives/cube-primitive.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/bigger/primitives/dynamic-mesh-primitive.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/bigger/primitives/mesh-primitive.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/bigger/primitives/plane-primitive.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/bigger/primitives/sphere-primitive.hpp
)
set(sources
  ${CMAKE_CURRENT_SOURCE_DIR}/src/app.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/camera.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/dynamic-mesh-primitive.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/mesh-primitive.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/utils.cpp
)
set(shaders
  ${CMAKE_CURRENT_SOURCE_DIR}/shaders/blinnphong/vs_blinnphong.sc
  ${CMAKE_CURRENT_SOURCE_DIR}/shaders/blinnphong/fs_blinnphong.sc
  ${CMAKE_CURRENT_SOURCE_DIR}/shaders/blinnphong/varying.def.sc
)

add_library(bigger STATIC ${headers} ${sources} ${shaders})
target_link_libraries(bigger PUBLIC bigg rand-util string-util tinyobjloader)
target_include_directories(bigger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(BIGGER_SHADER_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/shaders CACHE STRING "")

add_shader(${CMAKE_CURRENT_SOURCE_DIR}/shaders/blinnphong/vs_blinnphong.sc VERTEX   OUTPUT ${BIGGER_SHADER_OUTPUT_DIR} DX11_MODEL 5_0 GLSL 130)
add_shader(${CMAKE_CURRENT_SOURCE_DIR}/shaders/blinnphong/fs_blinnphong.sc FRAGMENT OUTPUT ${BIGGER_SHADER_OUTPUT_DIR} DX11_MODEL 5_0 GLSL 130)

option(BIGGER_EXAMPLES ON "")
if(BIGGER_EXAMPLES)
  add_executable(minimal ${CMAKE_CURRENT_SOURCE_DIR}/examples/minimal/main.cpp)
  target_link_libraries(minimal bigger)

  add_executable(cubes ${CMAKE_CURRENT_SOURCE_DIR}/examples/cubes/main.cpp)
  target_link_libraries(cubes bigger)
  add_custom_command(TARGET cubes POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${BIGGER_SHADER_OUTPUT_DIR} $<TARGET_FILE_DIR:cubes>/shaders)

  add_executable(mesh ${CMAKE_CURRENT_SOURCE_DIR}/examples/mesh/main.cpp)
  target_link_libraries(mesh bigger)
  add_custom_command(TARGET mesh POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${BIGGER_SHADER_OUTPUT_DIR} $<TARGET_FILE_DIR:mesh>/shaders)
  add_custom_command(TARGET mesh POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:mesh>/assets)
endif()
