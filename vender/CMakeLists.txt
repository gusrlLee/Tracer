cmake_minimum_required(VERSION 3.15)

# Metal API library for C++ 
add_subdirectory(metal-cpp)

# GLFW library for window
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)

# spdlog for logging 
add_subdirectory(spdlog)

# tinyglTF library 
add_subdirectory(tinygltf)
