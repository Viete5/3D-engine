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

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

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

    glEnable(GL_DEPTH_TEST);

    Shader shaderProgram("C:/prog/C++/openGL/shaders/shader.vert", "C:/prog/C++/openGL/shaders/shader.frag");

    // Вершины
    float vertices[] = {
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
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        // нижняя
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        // правая
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        // левая
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
    };

    unsigned int indices[] = {
        0, 1, 2,  2, 3, 0,
        4, 5, 6,  6, 7, 4,
        8, 9, 10,  10, 11, 8,
        12, 13, 14,  14, 15, 12, 
        16, 17, 18,  18, 19, 16,
        20, 21, 22,  22, 23, 20
    };

    VAO VAO1;
    VAO1.Bind();
    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 2, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 3, GL_FLOAT, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    Texture coolTexture("C:/prog/C++/openGL/resource/cat.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    shaderProgram.Activate();
    coolTexture.texUnit(shaderProgram, "ourTexture", 0);

    GLint modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
    GLint viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
    GLint projLoc = glGetUniformLocation(shaderProgram.ID, "projection");

    // --- RENDER LOOP ---
    while (!glfwWindowShouldClose(window))
    {
        // DeltaTime расчет
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        // Логика вращения
        if (isRotating) {
            rotationAngle +=  deltaTime; // Скорость вращения
        }

        // glClearColor(0.f, 0.3f, 0.3f, 1.0f);
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
        Matrix4 rotYMat = Matrix4::rotateY(rotationAngle);
        Matrix4 rotXMat = Matrix4::rotateX(rotationAngle); 

        //матрица преобразования из локальной системы координат в камерную
        Matrix4 model = transMat * (rotYMat * rotXMat);
        
        // Передача в шейдер. GL_TRUE транспонирует матрицы (важно для твоего translate)
        glUniformMatrix4fv(projLoc, 1, GL_TRUE, &projection.at(0,0));
        glUniformMatrix4fv(viewLoc, 1, GL_TRUE, &view.at(0,0));
        glUniformMatrix4fv(modelLoc, 1, GL_TRUE, &model.at(0,0));

        VAO1.Bind();
        glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(unsigned int), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    shaderProgram.Delete();
    coolTexture.Delete();
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
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
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}