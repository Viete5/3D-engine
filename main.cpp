#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "headers/shader.h"
#include "headers/VAO.h"
#include "headers/VBO.h"
#include "headers/EBO.h"
#include "headers/Texture.h"


#include "headers/Matrix.h"
#include "headers/Vector.h"
#include "headers/Torus.h"

#include <iostream>
#include <cmath>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// --- Глобальные переменные состояния ---
Vector cubePos(0.0f, 0.0f, 0.0f); // Позиция куба
float fov = 45.0f;                // Угол обзора (Zoom)

// Состояние вращения
bool isRotating = true;
float rotationAngle = 0.0f;
bool spacePressedLastFrame = false;

// Тайминг
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//light
Vector lightPos(5.0f, 5.0f, 0.0f);   
Vector lightColor(1.0f, 1.0f, 1.0f);

// --- Состояние переключения фигуры ---
enum class RenderMode { Cube, Torus };
RenderMode currentMode = RenderMode::Cube;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window, RenderMode& mode);

int main()
{
    // 1. Инициализация GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "My 3D Engine", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }



    Shader shaderProgram("C:/prog/C++/openGL/shaders/shader.vert", "C:/prog/C++/openGL/shaders/shader.frag");

    // Вершины
    float CubeVertices[] = {
        // Передняя
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        // Задняя
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
        // верхняя
        0.5f, 0.5f, 0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
         0.5f, 0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
         -0.5f,  0.5f, 0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        // нижняя
        0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
         0.5f, -0.5f,  -0.5f,  1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        -0.5f,  -0.5f,  0.5f,  0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        -0.5f,  -0.5f,  -0.5f,  0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        // правая
        0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        // левая
        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
         -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
         -0.5f,  0.5f, 0.5f,  0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
    };

    unsigned int CubeIndices[] = {
        0, 1, 2,  2, 3, 0,
        4, 7, 6,  6, 5, 4,
        10, 8, 9,  9, 11, 10,
        14, 13, 12,  14, 15, 13, 
        16, 17, 18,  19, 18, 17,
        20, 22, 23,  23, 21, 20
    };

    //back-face culling
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);


    //z-buffer
    glEnable(GL_DEPTH_TEST);



    VAO cubeVAO1;
    cubeVAO1.Bind();
    VBO cubeVBO1(CubeVertices, sizeof(CubeVertices));
    EBO cubeEBO1(CubeIndices, sizeof(CubeIndices));
    cubeVAO1.LinkAttrib(cubeVBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    cubeVAO1.LinkAttrib(cubeVBO1, 1, 2, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    cubeVAO1.LinkAttrib(cubeVBO1, 2, 3, GL_FLOAT, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    cubeVAO1.Unbind();
    cubeVBO1.Unbind();
    cubeEBO1.Unbind();

    Torus torus(1.0f, 0.4f, 50, 50);

    Texture coolTexture("C:/prog/C++/openGL/resource/white.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    shaderProgram.Activate();
    coolTexture.texUnit(shaderProgram, "ourTexture", 0);

    GLint modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
    GLint viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
    GLint projLoc = glGetUniformLocation(shaderProgram.ID, "projection");
    GLint normalMatrixLoc = glGetUniformLocation(shaderProgram.ID, "normalMatrix");
    GLint lightPosLoc = glGetUniformLocation(shaderProgram.ID, "lightPos");
    GLint lightColorLoc = glGetUniformLocation(shaderProgram.ID, "LightColour");

    // --- RENDER LOOP ---
    while (!glfwWindowShouldClose(window))
    {
        // DeltaTime расчет
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window,currentMode);

        // Логика вращения
        if (isRotating) {
            rotationAngle +=  deltaTime; // Скорость вращения
        }

        glClearColor(0.f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderProgram.Activate();
        coolTexture.Bind();

        // 1. PROJECTION (Зум)
        // Конвертируем FOV в радианы
        float fovRad = fov * (3.14159f / 180.0f);
        float aspect = (float)SCR_WIDTH / (float)SCR_HEIGHT;
        Matrix4 projection = Matrix4::Perspective(fovRad, aspect, 0.1f, 100.0f);
        
        // 2. VIEW
        Vector camPos(0.0f, 0.0f, 3.0f); 
        Vector target(0.0f, 0.0f, 0.0f); 
        Vector up(0.0f, 1.0f, 0.0f);     
        Matrix4 view = Matrix4::lookAt(camPos, target, up);

        // 3. MODEL
        Matrix4 transMat = Matrix4::translate(cubePos.getX(), cubePos.getY(), cubePos.getZ());
        Matrix4 rotYMat = Matrix4::rotateY(rotationAngle*0.5f);
        Matrix4 rotXMat = Matrix4::rotateX(rotationAngle); 

        //матрица преобразования из локальной системы координат в камерную
        Matrix4 model = transMat * (rotYMat * rotXMat);

        //матрица для расчета света
        Matrix4 normalMatrix = model.normMatrix();
        
        // Передача в шейдер. 
        glUniform3f(lightPosLoc, lightPos.getX(), lightPos.getY(), lightPos.getZ());
        glUniform3f(lightColorLoc, lightColor.getX(), lightColor.getY(), lightColor.getZ());
        glUniformMatrix4fv(projLoc, 1, GL_TRUE, &projection.at(0,0));
        glUniformMatrix4fv(viewLoc, 1, GL_TRUE, &view.at(0,0));
        glUniformMatrix4fv(modelLoc, 1, GL_TRUE, &model.at(0,0));
        glUniformMatrix4fv(normalMatrixLoc, 1, GL_TRUE, &normalMatrix.at(0,0));
        if (currentMode==RenderMode::Torus) {
            torus.Draw();
        }
        else {
            cubeVAO1.Bind();
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }    
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    cubeVAO1.Delete();
    cubeVBO1.Delete();
    cubeEBO1.Delete();
    shaderProgram.Delete();
    coolTexture.Delete();
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window, RenderMode& mode)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float speed = 2.5f * deltaTime;

    //движение стрелки
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        cubePos.setY(cubePos.getY()+speed);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        cubePos.setY(cubePos.getY()-speed);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        cubePos.setX(cubePos.getX()-speed);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        cubePos.setX(cubePos.getX()+speed);


    // Зум (Q / E)
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        fov -= 40.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        fov += 40.0f * deltaTime;

    // Ограничения зума
    if (fov < 1.0f) fov = 1.0f;
    if (fov > 90.0f) fov = 90.0f;

    // Вращение (Space) - переключатель
    bool spacePressed = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
    if (spacePressed && !spacePressedLastFrame) {
        isRotating = !isRotating;
    }
    spacePressedLastFrame = spacePressed;
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        mode = RenderMode::Cube;
        
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        mode = RenderMode::Torus;
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}