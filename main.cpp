#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "headers/shader.h"
#include "headers/VAO.h"
#include "headers/VBO.h"
#include "headers/EBO.h"
#include "headers/Texture.h"
#include "headers/Cube.h"


#include "headers/Matrix.h"
#include "headers/Vector.h"
#include "headers/Torus.h"

#include <iostream>
#include <cmath>

const unsigned int SCR_WIDTH = 800;   // Screen width
const unsigned int SCR_HEIGHT = 600;   // Screen height

// Global variables
Vector cubePos(0.0f, 0.0f, 0.0f); // Cube pos
float fov = 45.0f;                // Zoom

// Rotation condition
bool isRotating = true;
float rotationAngle = 0.0f;
bool spacePressedLastFrame = false;

// Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Light
Vector lightPos(5.0f, 0.0f, 0.0f);   
Vector lightColor(1.0f, 1.0f, 1.0f);

// Change the figure 
enum class RenderMode { Cube, Torus };
RenderMode currentMode = RenderMode::Cube;

// Functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window, RenderMode& mode);
void initializeglfw();
GLFWwindow* createwindow();

int main()
{
    // Initialize GLFW
    initializeglfw();

    // Create window
    GLFWwindow* window = createwindow();
    if (!window) return -1;

    // Check glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Shaders
    Shader shaderProgram("C:/prog/C++/openGL/shaders/shader.vert", "C:/prog/C++/openGL/shaders/shader.frag");
    Shader shadowShader("C:/prog/C++/openGL/shadow/shadow.vert", "C:/prog/C++/openGL/shadow/shadow.frag");

    // Cube
    Cube Cube1(Vector(0.0f,0.0f,0.0f));
    Cube Cube2(Vector(0.0f,0.0f,0.0f));

    //back-face culling
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);


    //z-buffer
    glEnable(GL_DEPTH_TEST);

    //SHADOW
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);


    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH,
    SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE) {
        std::cout<<"DEPTH BUFFER ERROR";
        return -1;
    }

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

    GLint depthMatrixID = glGetUniformLocation(shadowShader.ID, "depthMVP");
    GLint lightSpaceLoc = glGetUniformLocation(shadowShader.ID, "lightSpaceMatrix");
    GLint modelShadowLoc = glGetUniformLocation(shadowShader.ID, "model");

    // --- RENDER LOOP ---
    while (!glfwWindowShouldClose(window))
    {
        processInput(window,currentMode);

        //ВЫЧИСЛЕНИЯ

        // DeltaTime расчет
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Логика вращения
        if (isRotating) {
            rotationAngle +=  deltaTime; // Скорость вращения
        }

        // Cubes
        Cube1.setPosition(cubePos);
        Cube1.SetRotation(rotationAngle, rotationAngle * 0.5f);

        Cube2.SetRotation(0.0f, rotationAngle);

        // 1. PROJECTION (Зум)
        // Конвертируем FOV в радианы
        float fovRad = fov * (3.14159f / 180.0f);
        float aspect = (float)SCR_WIDTH / (float)SCR_HEIGHT;
        Matrix4 projection = Matrix4::Perspective(fovRad, aspect, 0.01f, 500.0f);
        
        // 2. VIEW
        Vector camPos(0.0f, 0.0f, 3.0f); 
        Vector target(0.0f, 0.0f, 0.0f); 
        Vector up(0.0f, 1.0f, 0.0f);     
        Matrix4 view = Matrix4::lookAt(camPos, target, up);


        //матрицы для shade
        Matrix4 ortho = Matrix4::ortho(-10.0f,10.0f,-10.0f,10.0f,0.01f,500.0f);
        Matrix4 lightView = Matrix4::lookAt(lightPos,target,up);

        Matrix4 lightSpaceMatrix = ortho * lightView ;

        //первый проход
        // glCullFace(GL_FRONT);

        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glClear(GL_DEPTH_BUFFER_BIT);
        shadowShader.Activate();
        glUniformMatrix4fv(lightSpaceLoc, 1, GL_TRUE, &lightSpaceMatrix.at(0,0));
        
        glDisable(GL_CULL_FACE);
        if (currentMode==RenderMode::Torus) {
            Matrix4 modelTorus = Matrix4::translate(0.0f, 0.0f, 0.0f);
            modelTorus = modelTorus * Matrix4::rotateX(rotationAngle) * Matrix4::rotateY(rotationAngle*0.5f); // Добавим вращение тору
            glUniformMatrix4fv(modelShadowLoc, 1, GL_TRUE, &modelTorus.at(0,0));
            torus.Draw();
        }
        else {
            Cube1.draw(shadowShader);
            // Cube2.draw(shadowShader);
        }  
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        //второй проход
        glEnable(GL_CULL_FACE);

        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderProgram.Activate();
        glActiveTexture(GL_TEXTURE0);
        coolTexture.Bind();

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glUniform1i(glGetUniformLocation(shaderProgram.ID, "shadowMap"), 1);
        
        // Передача в шейдер. 
        glUniform3f(lightPosLoc, lightPos.getX(), lightPos.getY(), lightPos.getZ());
        glUniform3f(lightColorLoc, lightColor.getX(), lightColor.getY(), lightColor.getZ());
        glUniformMatrix4fv(projLoc, 1, GL_TRUE, &projection.at(0,0));
        glUniformMatrix4fv(viewLoc, 1, GL_TRUE, &view.at(0,0));
        GLint lsLocMain = glGetUniformLocation(shaderProgram.ID, "lightSpaceMatrix");
        glUniformMatrix4fv(lsLocMain, 1, GL_TRUE, &lightSpaceMatrix.at(0,0));
        if (currentMode==RenderMode::Torus) {
            Matrix4 modelTorus = Matrix4::translate(0.0f, 0.0f, 0.0f);
            modelTorus = modelTorus * Matrix4::rotateX(rotationAngle) * Matrix4::rotateY(rotationAngle*0.5f);
            Matrix4 normalMatrixTorus = modelTorus.normMatrix();

            glUniformMatrix4fv(modelLoc, 1, GL_TRUE, &modelTorus.at(0,0));
            glUniformMatrix4fv(normalMatrixLoc, 1, GL_TRUE, &normalMatrixTorus.at(0,0));
            torus.Draw();
        }
        else {
            Cube1.draw(shaderProgram);
            // Cube2.draw(shaderProgram);  
        }    
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


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

void initializeglfw() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

GLFWwindow* createwindow() {
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "My 3D Engine", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    return window;
}