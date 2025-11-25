cmake_minimum_required(VERSION 3.10)
project(OpenGL_Project CXX)
set(CMAKE_C_COMPILER "C:/msys64/mingw64/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "C:/msys64/mingw64/bin/g++.exe")

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


set(GLFW_ROOT "C:/prog/glfw-3.4.bin.WIN64") 

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${GLFW_ROOT})

find_package(glfw3 CONFIG REQUIRED)

# -----------------------------------------------------------
# ИЗМЕНЕНИЕ: Только main.cpp для компиляции
# -----------------------------------------------------------
add_executable(${PROJECT_NAME}
    main.cpp
    shaders/Shader.cpp
    shaders/EBO.cpp
    shaders/VAO.cpp
    shaders/VBO.cpp
    shaders/stb.cpp
    shaders/Texture.cpp
    Math/Matrix.cpp
    Math/Vector.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    vendor/include  # Заголовки GLAD
    ${GLFW_ROOT}/include # Заголовки GLFW
)


target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw
    opengl32
    gdi32 
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad.o 
)