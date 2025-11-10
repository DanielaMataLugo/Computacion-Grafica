//Práctica 12
//Daniela Mata Lugo
//Fecha de entrega: 09 de noviembre de 2025
//319302149

#include <iostream>
#include <cmath>
#include <fstream>
#include <algorithm>
#include <cstdio>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Load Models
#include "SOIL2/SOIL2.h"

#include "Shader.h"
#include "Camera.h"
#include "Model.h"


void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void Animation();
bool SaveAnimation(const char* path);
bool LoadAnimation(const char* path);
void saveFrame(void);
void resetElements(void);
void interpolation(void);

const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0f;
GLfloat lastY = HEIGHT / 2.0f;
bool keys[1024] = { false };
bool firstMouse = true;

// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active = false;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(0.0f, 2.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f)
};

float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

glm::vec3 Light1 = glm::vec3(0);

float rotBall = 0.0f;
float rotDog = 0.0f;
int dogAnim = 0;
float FLegs = 0.0f;
float RLegs = 0.0f;
float legFL = 0.0f; 
float legFR = 0.0f; 
float legBL = 0.0f; 
float legBR = 0.0f; 
float head = 0.0f;
float tail = 0.0f;
float rotXDog = 0.0f;
float dogPosX = 0.0f;
float dogPosY = 0.0f;
float dogPosZ = 0.0f;

#define MAX_FRAMES 200
int i_max_steps = 190;
int i_curr_steps = 0;

typedef struct _frame {
    float rotDog;   
    float rotDogInc;
    float dogPosX;  
    float dogPosY;  
    float dogPosZ;  
    float incX; 
    float incY; 
    float incZ;
    float head;     
    float headInc;
    float legFL;    
    float legFLInc;
    float legFR;    
    float legFRInc;
    float legBL;    
    float legBLInc;
    float legBR;    
    float legBRInc;
    float tail;     
    float tailInc;
} FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;            
bool play = false;
int playIndex = 0;

const char* ANIM_FILE = "dog_anim.txt";

// Deltatime
GLfloat deltaTime = 0.0f;  // Time between current frame and last frame
GLfloat lastFrame = 0.0f;  // Time of last frame

bool SaveAnimation(const char* path) {
    std::ofstream ofs(path);
    if (!ofs.is_open()) return false;

    ofs << FrameIndex << " " << i_max_steps << "\n";
    for (int i = 0; i < FrameIndex; ++i) {
        const FRAME& k = KeyFrame[i];
        ofs << k.rotDog << " "
            << k.dogPosX << " " << k.dogPosY << " " << k.dogPosZ << " "
            << k.head << " "
            << k.legFL << " " << k.legFR << " "
            << k.legBL << " " << k.legBR << " "
            << k.tail
            << "\n";
    }
    return true;
}

bool LoadAnimation(const char* path) {
    std::ifstream ifs(path);
    if (!ifs.is_open()) return false;

    int count = 0;
    ifs >> count >> i_max_steps;
    if (!ifs.good()) return false;

    FrameIndex = std::min(count, (int)MAX_FRAMES);

    for (int i = 0; i < FrameIndex; ++i) {
        FRAME k{};
        ifs >> k.rotDog
            >> k.dogPosX >> k.dogPosY >> k.dogPosZ
            >> k.head
            >> k.legFL >> k.legFR
            >> k.legBL >> k.legBR
            >> k.tail;

        if (!ifs.good()) { FrameIndex = i; break; }
        k.incX = k.incY = k.incZ = 0.0f;
        k.rotDogInc = 0.0f;
        k.headInc = 0.0f;
        k.legFLInc = k.legFRInc = k.legBLInc = k.legBRInc = 0.0f;
        k.tailInc = 0.0f;

        KeyFrame[i] = k;
    }

    if (FrameIndex > 0) resetElements();
    return true;
}

void saveFrame(void) {
    std::printf("frameindex %d\n", FrameIndex);

    KeyFrame[FrameIndex].dogPosX = dogPosX;
    KeyFrame[FrameIndex].dogPosY = dogPosY;
    KeyFrame[FrameIndex].dogPosZ = dogPosZ;
    KeyFrame[FrameIndex].rotDog = rotDog;
    KeyFrame[FrameIndex].head = head;
    KeyFrame[FrameIndex].legFL = legFL;
    KeyFrame[FrameIndex].legFR = legFR;
    KeyFrame[FrameIndex].legBL = legBL;
    KeyFrame[FrameIndex].legBR = legBR;
    KeyFrame[FrameIndex].tail = tail;

    FrameIndex++;
}

void resetElements(void) {
    dogPosX = KeyFrame[0].dogPosX;
    dogPosY = KeyFrame[0].dogPosY;
    dogPosZ = KeyFrame[0].dogPosZ;
    head = KeyFrame[0].head;
    rotDog = KeyFrame[0].rotDog;
    legFL = KeyFrame[0].legFL;
    legFR = KeyFrame[0].legFR;
    legBL = KeyFrame[0].legBL;
    legBR = KeyFrame[0].legBR;
    tail = KeyFrame[0].tail;
}

void interpolation(void) {
    KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].dogPosX - KeyFrame[playIndex].dogPosX) / i_max_steps;
    KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].dogPosY - KeyFrame[playIndex].dogPosY) / i_max_steps;
    KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].dogPosZ - KeyFrame[playIndex].dogPosZ) / i_max_steps;
    KeyFrame[playIndex].headInc = (KeyFrame[playIndex + 1].head - KeyFrame[playIndex].head) / i_max_steps;
    KeyFrame[playIndex].rotDogInc = (KeyFrame[playIndex + 1].rotDog - KeyFrame[playIndex].rotDog) / i_max_steps;
    KeyFrame[playIndex].legFLInc = (KeyFrame[playIndex + 1].legFL - KeyFrame[playIndex].legFL) / i_max_steps;
    KeyFrame[playIndex].legFRInc = (KeyFrame[playIndex + 1].legFR - KeyFrame[playIndex].legFR) / i_max_steps;
    KeyFrame[playIndex].legBLInc = (KeyFrame[playIndex + 1].legBL - KeyFrame[playIndex].legBL) / i_max_steps;
    KeyFrame[playIndex].legBRInc = (KeyFrame[playIndex + 1].legBR - KeyFrame[playIndex].legBR) / i_max_steps;
    KeyFrame[playIndex].tailInc = (KeyFrame[playIndex + 1].tail - KeyFrame[playIndex].tail) / i_max_steps;
}

int main() {
    // Init GLFW
    glfwInit();

    // Create a GLFWwindow object
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Daniela Mata", nullptr, nullptr);
    if (nullptr == window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Callbacks
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit()) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
    Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

    // Models
    Model DogBody((char*)"Models/DogBody.obj");
    Model HeadDog((char*)"Models/HeadDog.obj");
    Model DogTail((char*)"Models/TailDog.obj");
    Model F_RightLeg((char*)"Models/F_RightLegDog.obj");
    Model F_LeftLeg((char*)"Models/F_LeftLegDog.obj");
    Model B_RightLeg((char*)"Models/B_RightLegDog.obj");
    Model B_LeftLeg((char*)"Models/B_LeftLegDog.obj");
    Model Piso((char*)"Models/piso.obj");
    Model Ball((char*)"Models/ball.obj");

    
    for (int i = 0; i < MAX_FRAMES; i++) {
        KeyFrame[i].dogPosX = 0;  
        KeyFrame[i].dogPosY = 0;  
        KeyFrame[i].dogPosZ = 0;
        KeyFrame[i].incX = 0;     
        KeyFrame[i].incY = 0;     
        KeyFrame[i].incZ = 0;
        KeyFrame[i].rotDog = 0;   
        KeyFrame[i].rotDogInc = 0;
        KeyFrame[i].legFL = 0;    KeyFrame[i].legFLInc = 0;
        KeyFrame[i].legFR = 0;    KeyFrame[i].legFRInc = 0;
        KeyFrame[i].legBL = 0;    KeyFrame[i].legBLInc = 0;
        KeyFrame[i].legBR = 0;    KeyFrame[i].legBRInc = 0;
        KeyFrame[i].tail = 0;    KeyFrame[i].tailInc = 0;
        KeyFrame[i].head = 0;    KeyFrame[i].headInc = 0;
    }

    
    if (LoadAnimation(ANIM_FILE)) {
        std::cout << "Animacion cargada desde " << ANIM_FILE << " con " << FrameIndex << " frames.\n";
    }
    else {
        std::cout << "No se encontro animacion previa. Empezando en blanco.\n";
    }

    // Buffers
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    lightingShader.Use();
    glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
    glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

    glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

    // Game loop
    while (!glfwWindowShouldClose(window)) {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        DoMovement();
        Animation();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        glm::mat4 modelTemp = glm::mat4(1.0f);

        lightingShader.Use();
        glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);

        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        // Directional light
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.6f, 0.6f, 0.6f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.3f, 0.3f, 0.3f);

        // Point light 1
        glm::vec3 lightColor;
        lightColor.x = std::abs(std::sin(glfwGetTime() * Light1.x));
        lightColor.y = std::abs(std::sin(glfwGetTime() * Light1.y));
        lightColor.z = std::sin(glfwGetTime() * Light1.z);

        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 0.2f, 0.2f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);

        // SpotLight
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.2f, 0.2f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.2f, 0.2f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));

        // Material
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

        // Camera matrices
        glm::mat4 view = camera.GetViewMatrix();
        GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
        GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glm::mat4 model(1);

        // Piso
        view = camera.GetViewMatrix();
        model = glm::mat4(1);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Piso.Draw(lightingShader);

        // Perro: cuerpo
        model = glm::mat4(1);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
        model = glm::rotate(model, glm::radians(rotXDog), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::translate(model, glm::vec3(dogPosX, dogPosY, dogPosZ));
        model = glm::rotate(model, glm::radians(rotDog), glm::vec3(0.0f, 1.0f, 0.0f));
        modelTemp = model;
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        DogBody.Draw(lightingShader);

        // Cabeza
        model = modelTemp;
        model = glm::translate(model, glm::vec3(0.0f, 0.093f, 0.208f));
        model = glm::rotate(model, glm::radians(head), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        HeadDog.Draw(lightingShader);

        // Cola
        model = modelTemp;
        model = glm::translate(model, glm::vec3(0.0f, 0.026f, -0.288f));
        model = glm::rotate(model, glm::radians(tail), glm::vec3(0.0f, 0.0f, -1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        DogTail.Draw(lightingShader);

        // Pata delantera izquierda
        model = modelTemp;
        model = glm::translate(model, glm::vec3(0.112f, -0.044f, 0.074f));
        model = glm::rotate(model, glm::radians(legFL), glm::vec3(-1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        F_LeftLeg.Draw(lightingShader);

        // Pata delantera derecha
        model = modelTemp;
        model = glm::translate(model, glm::vec3(-0.111f, -0.055f, 0.074f));
        model = glm::rotate(model, glm::radians(legFR), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        F_RightLeg.Draw(lightingShader);

        // Pata trasera izquierda
        model = modelTemp;
        model = glm::translate(model, glm::vec3(0.082f, -0.046f, -0.218f));
        model = glm::rotate(model, glm::radians(legBL), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        B_LeftLeg.Draw(lightingShader);

        // Pata trasera derecha
        model = modelTemp;
        model = glm::translate(model, glm::vec3(-0.083f, -0.057f, -0.231f));
        model = glm::rotate(model, glm::radians(legBR), glm::vec3(-1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        B_RightLeg.Draw(lightingShader);

        // Esfera
        model = glm::mat4(1);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
        model = glm::rotate(model, glm::radians(rotBall), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Ball.Draw(lightingShader);
        glDisable(GL_BLEND);
        glBindVertexArray(0);

        lampShader.Use();
        modelLoc = glGetUniformLocation(lampShader.Program, "model");
        viewLoc = glGetUniformLocation(lampShader.Program, "view");
        projLoc = glGetUniformLocation(lampShader.Program, "projection");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        model = glm::mat4(1);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        model = glm::mat4(1);
        model = glm::translate(model, pointLightPositions[0]);
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

void DoMovement() {
    
    if (keys[GLFW_KEY_8]) head += 0.05f;
    if (keys[GLFW_KEY_7]) head -= 0.05f;

    if (keys[GLFW_KEY_9]) legFL += 0.05f;
    if (keys[GLFW_KEY_6]) legFL -= 0.05f;

    if (keys[GLFW_KEY_5]) legFR += 0.01f; 
    if (keys[GLFW_KEY_4]) legFR -= 0.01f;

    if (keys[GLFW_KEY_3]) legBL += 0.05f;
    if (keys[GLFW_KEY_2]) legBL -= 0.05f;

    if (keys[GLFW_KEY_Q]) legBR += 0.05f;
    if (keys[GLFW_KEY_I]) legBR -= 0.05f;

    if (keys[GLFW_KEY_N]) tail += 0.05f;
    if (keys[GLFW_KEY_M]) tail -= 0.05f;

    if (keys[GLFW_KEY_Z]) rotXDog += 0.005f;
    if (keys[GLFW_KEY_X]) rotXDog -= 0.05f;

    if (keys[GLFW_KEY_F]) { rotDog += 0.01f; }
    if (keys[GLFW_KEY_U]) { rotDog -= 0.01f; }

    if (keys[GLFW_KEY_H]) { dogPosZ += 0.001f; }
    if (keys[GLFW_KEY_Y]) { dogPosZ -= 0.01f; }

    if (keys[GLFW_KEY_G]) { dogPosX -= 0.01f; }
    if (keys[GLFW_KEY_J]) { dogPosX += 0.01f; }

    if (keys[GLFW_KEY_0]) { dogPosY -= 0.001f; }
    if (keys[GLFW_KEY_1]) { dogPosY += 0.01f; }

    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])    camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])  camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])  camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) camera.ProcessKeyboard(RIGHT, deltaTime);

    if (keys[GLFW_KEY_T]) { pointLightPositions[0].x += 0.01f; }
    if (keys[GLFW_KEY_G]) { pointLightPositions[0].x -= 0.01f; }
    if (keys[GLFW_KEY_Y]) { pointLightPositions[0].y += 0.01f; }
    if (keys[GLFW_KEY_H]) { pointLightPositions[0].y -= 0.01f; }
    if (keys[GLFW_KEY_O]) { pointLightPositions[0].z -= 0.1f; }
    if (keys[GLFW_KEY_J]) { pointLightPositions[0].z += 0.01f; }
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS)      keys[key] = true;
        else if (action == GLFW_RELEASE) keys[key] = false;
    }
    if (key == GLFW_KEY_L && action == GLFW_PRESS) {
        if (!play && (FrameIndex > 1)) {
            resetElements();
            interpolation();
            play = true;
            playIndex = 0;
            i_curr_steps = 0;
        }
        else {
            play = false;
        }
    }
    if (key == GLFW_KEY_K && action == GLFW_PRESS) {
        if (FrameIndex < MAX_FRAMES) {
            saveFrame();
        }
    }
    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        if (SaveAnimation(ANIM_FILE))
            std::cout << "Animacion guardada en " << ANIM_FILE << "\n";
        else
            std::cout << "ERROR: no se pudo guardar " << ANIM_FILE << "\n";
    }
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        active = !active;
        if (active) {
            Light1 = glm::vec3(0.2f, 0.8f, 1.0f);
        }
        else {
            Light1 = glm::vec3(0.0f);
        }
    }
}

void Animation() {
    if (play) {
        if (i_curr_steps >= i_max_steps) { 
            playIndex++;
            if (playIndex > FrameIndex - 2) {
                std::printf("termina anim\n");
                playIndex = 0;
                play = false;
            }
            else {
                i_curr_steps = 0;
                interpolation();
            }
        }
        else {
            
            dogPosX += KeyFrame[playIndex].incX;
            dogPosY += KeyFrame[playIndex].incY;
            dogPosZ += KeyFrame[playIndex].incZ;
            head += KeyFrame[playIndex].headInc;
            rotDog += KeyFrame[playIndex].rotDogInc;
            legFL += KeyFrame[playIndex].legFLInc;
            legFR += KeyFrame[playIndex].legFRInc;
            legBL += KeyFrame[playIndex].legBLInc;
            legBR += KeyFrame[playIndex].legBRInc;
            tail += KeyFrame[playIndex].tailInc;

            i_curr_steps++;
        }
    }
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos) {
    if (firstMouse) {
        lastX = (GLfloat)xPos;
        lastY = (GLfloat)yPos;
        firstMouse = false;
    }

    GLfloat xOffset = (GLfloat)xPos - lastX;
    GLfloat yOffset = lastY - (GLfloat)yPos;  

    lastX = (GLfloat)xPos;
    lastY = (GLfloat)yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}
