cmake_minimum_required(VERSION 3.10)
project(OpenGL_Project CXX)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(GLFW_ROOT "$ENV{GLFW_ROOT}" CACHE PATH "Optional path to a glfw3 installation root")

if(GLFW_ROOT)
    list(APPEND CMAKE_PREFIX_PATH "${GLFW_ROOT}")
endif()

find_package(glfw3 CONFIG REQUIRED)

set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "Disable assimp tools" FORCE)
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "Disable assimp samples" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "Disable assimp tests" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "Disable assimp install target" FORCE)
add_subdirectory(third_party/assimp EXCLUDE_FROM_ALL)

add_subdirectory(engine)
add_subdirectory(apps/sandbox)
