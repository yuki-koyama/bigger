cmake_minimum_required(VERSION 3.1)

project(bigger CXX)

file(GLOB headers ${CMAKE_CURRENT_SOURCE_DIR}/include/bigger/*.hpp)
file(GLOB sources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(bigger STATIC ${headers} ${sources})
target_include_directories(bigger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
