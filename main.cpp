#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "headers/shader.h"
#include "headers/Texture.h"
#include "headers/Cube.h"
#include "headers/Matrix.h"
#include "headers/Vector.h"
#include "headers/Torus.h"
#include "headers/Camera.h"
#include "headers/Light.h"
#include "headers/ShadowMap.h"
#include "headers/Object.h"


// Global variables
float fov = 60.0f;       

const unsigned int SCR_WIDTH = 800;   
const unsigned int SCR_HEIGHT = 600; 

// Rotation condition
bool isRotating = true;
float rotationAngle = 0.0f;
bool spacePressedLastFrame = false;

// Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;


// Functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
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
    Shader shaderProgram("../shaders/shader.vert", "../shaders/shader.frag");
    Shader shadowShader("../shadow/shadow.vert", "../shadow/shadow.frag");

    shaderProgram.Activate();
    

    // Objects
    Cube Cube1;
    Cube Cube2;
    Cube Cube3;

    Cube1.SetPosition(Vector(-0.5f,1.0f,-1.5f));
    Cube2.SetPosition(Vector(1.5f,2.0f,-5.0f));
    Cube3.SetPosition(Vector(2.0f,0.0f,0.0f));
    
    Cube1.SetRotation(Vector(0.5f, 1.0f, 2.0f));
    Cube3.SetRotation(Vector(0.0f,2.0f,4.0f));

    Cube2.SetBaseColor(Vector4(0.2f,0.66f,0.19f,1.0f));
    
    Cube2.SetScale(Vector(0.5f,0.5f,0.5f));
    Cube1.SetScale(Vector(0.25f,0.25f,0.25f));

    Texture Texture1("C:/prog/C++/openGL/resource/cat.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    Texture Texture2("C:/prog/C++/openGL/resource/hippo.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    Texture Texture3("C:/prog/C++/openGL/resource/damn.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);

    Cube1.SetObjTex(&Texture1);
    Cube2.SetObjTex(&Texture2);
    Cube3.SetObjTex(&Texture3);

    Torus torus(1.0f, 0.4f, 50, 50);

    torus.SetPosition(Vector(-1.0f,-1.0f,-1.0f));
    torus.SetScale(Vector(0.5f,0.5f,0.5f));
    torus.SetBaseColor(Vector4(1.0f,0.2f,0.1f,1.0f));


    //back-face culling
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);


    //z-buffer
    glEnable(GL_DEPTH_TEST);

    // ShadowMap
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    ShadowMap shadMap(SHADOW_WIDTH, SHADOW_HEIGHT);
    shaderProgram.setInt("shadowMap", 1);

    // Light
    Vector lightPos(0,0,0);
    Vector lightCol(1,1,1);
    Light Light1(lightPos, lightCol);

    
    // Camera
    Vector camPos(0,0,3);
    Vector target(0,0,0);
    Camera cam(camPos, target, 45.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT);


    glfwSetWindowUserPointer(window, &cam);


    // --- RENDER LOOP ---
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // Calculations

        // DeltaTime 
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Rotation
        if (isRotating) {
            rotationAngle +=  deltaTime; 
        }

        // Cubes
        Cube1.SetRotation(Vector(rotationAngle, rotationAngle * 0.5f, rotationAngle));

        Cube2.SetRotation(Vector(0.0f, rotationAngle,0.0f));

        torus.SetRotation(Vector(rotationAngle, rotationAngle * 0.5f, 0.0f));

        // Camera and light
        cam.FOV = fov;
        Matrix4 lightSpaceMatrix = Light1.GetLightSpaceMatrix();


        //FIRST PASS

        shadMap.BeginRender();

        shadowShader.Activate();
        shadowShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        
        glDisable(GL_CULL_FACE);

        torus.Draw(shadowShader);
        Cube1.Draw(shadowShader);
        Cube2.Draw(shadowShader);
        Cube3.Draw(shadowShader);



        shadMap.EndRender();

        // SECOND PASS
        glEnable(GL_CULL_FACE);


        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderProgram.Activate();
        glActiveTexture(GL_TEXTURE0);


        shadMap.Bind(GL_TEXTURE1);
        
        
        // Передача в шейдер. 
        shaderProgram.setVec3("lightPos", Light1.Position);
        shaderProgram.setVec3("LightColour", Light1.Color);
        shaderProgram.setMat4("projection", cam.GetProjectionMatrix());
        shaderProgram.setMat4("view", cam.GetViewMatrix());
        shaderProgram.setMat4("lightSpaceMatrix", lightSpaceMatrix);

        glActiveTexture(GL_TEXTURE0);
        torus.Draw(shaderProgram);
        Cube1.Draw(shaderProgram);
        Cube2.Draw(shaderProgram);
        Cube3.Draw(shaderProgram); 


        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float speed = 2.5f * deltaTime;

    //движение стрелки
    // if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    //     cubePos.setY(cubePos.getY()+speed);
    // if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    //     cubePos.setY(cubePos.getY()-speed);
    // if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    //     cubePos.setX(cubePos.getX()-speed);
    // if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    //     cubePos.setX(cubePos.getX()+speed);


    // Зум (Q / E)
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        fov -= 40.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        fov += 40.0f * deltaTime;

    // Ограничения зума
    if (fov < 0.5f) fov = 0.5f;
    if (fov > 110.0f) fov = 110.0f;

    // Вращение (Space) - переключатель
    // bool spacePressed = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
    // if (spacePressed && !spacePressedLastFrame) {
    //     isRotating = !isRotating;
    // }
    // spacePressedLastFrame = spacePressed;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // Обновляем матрицу проекции камеры при изменении размера окна
    Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (cam) {
        float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
        cam->UpdateAspectRatio(aspectRatio);
    }

    // Обновляем область вывода
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