add_library(engine STATIC
    src/assets/stb.cpp
    src/assets/modelLoader.cpp
    src/assets/textureLoader.cpp

    src/core/application.cpp
    src/core/log.cpp
    src/core/paths.cpp
    src/core/time.cpp

    src/math/matrix.cpp
    src/math/vector.cpp
    src/math/vector4.cpp

    src/platform/input.cpp
    src/platform/window.cpp

    src/render/material.cpp
    src/render/mesh.cpp
    src/render/model.cpp
    src/render/primitives.cpp
    src/render/renderer.cpp
    src/render/shader.cpp
    src/render/texture.cpp

    src/renderOpenGl/GLIndexBuffer.cpp
    src/renderOpenGl/GLShaderProgram.cpp
    src/renderOpenGl/GLTexture.cpp
    src/renderOpenGl/GLVertexArray.cpp
    src/renderOpenGl/GLVertexBuffer.cpp

    src/scene/camera.cpp
    src/scene/light.cpp
    src/scene/scene.cpp
    src/scene/sceneObject.cpp
    src/scene/transform.cpp
)

target_include_directories(engine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/vendor/include
)

target_link_libraries(engine
    PUBLIC
        glfw
        assimp::assimp
        opengl32
        gdi32
        ${PROJECT_SOURCE_DIR}/vendor/glad.o
)
