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
set(GLFW_VULKAN_STATIC      OFF CACHE INTERNAL "" FORCE)
set(LIB_SUFFIX              ""  CACHE INTERNAL "" FORCE)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/bigg)

file(GLOB headers ${CMAKE_CURRENT_SOURCE_DIR}/include/bigger/*.hpp)
file(GLOB sources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
set(shaders
  ${CMAKE_CURRENT_SOURCE_DIR}/shaders/blinnphong/vs_blinnphong.sc
  ${CMAKE_CURRENT_SOURCE_DIR}/shaders/blinnphong/fs_blinnphong.sc
  ${CMAKE_CURRENT_SOURCE_DIR}/shaders/blinnphong/varying.def.sc
)

add_library(bigger STATIC ${headers} ${sources} ${shaders})
target_link_libraries(bigger PUBLIC bigg)
target_include_directories(bigger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_shader(${CMAKE_CURRENT_SOURCE_DIR}/shaders/blinnphong/vs_blinnphong.sc VERTEX   OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/shaders DX11_MODEL 5_0 GLSL 130)
add_shader(${CMAKE_CURRENT_SOURCE_DIR}/shaders/blinnphong/fs_blinnphong.sc FRAGMENT OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/shaders DX11_MODEL 5_0 GLSL 130)

option(BIGGER_EXAMPLES ON "")
if(BIGGER_EXAMPLES)
  add_executable(cubes ${CMAKE_CURRENT_SOURCE_DIR}/examples/cubes/main.cpp)
  target_link_libraries(cubes bigger)
endif()
