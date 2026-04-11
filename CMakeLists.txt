cmake_minimum_required(VERSION 3.10)
project(OpenGL_Project CXX)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(GLFW_ROOT "$ENV{GLFW_ROOT}" CACHE PATH "Optional path to a glfw3 installation root")
set(ASSIMP_ROOT "$ENV{ASSIMP_ROOT}" CACHE PATH "Optional path to an assimp installation root")

if(GLFW_ROOT)
    list(APPEND CMAKE_PREFIX_PATH "${GLFW_ROOT}")
endif()

if(ASSIMP_ROOT)
    list(APPEND CMAKE_PREFIX_PATH "${ASSIMP_ROOT}")
endif()

find_package(glfw3 CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)

add_executable(${PROJECT_NAME}
    main.cpp
    additions/Camera.cpp
    shaders/Shader.cpp
    shaders/EBO.cpp
    shaders/VAO.cpp
    shaders/VBO.cpp
    shaders/stb.cpp
    shaders/Texture.cpp
    shaders/ShadowMap.cpp
    Math/Matrix.cpp
    Math/Vector.cpp
    Math/Torus.cpp
    Math/Cube.cpp
    Math/Object.cpp
    Math/Vector4.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    vendor/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw
    assimp::assimp
    opengl32
    gdi32
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad.o
)
