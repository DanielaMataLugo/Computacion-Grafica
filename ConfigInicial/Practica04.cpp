//Práctica 04
//Daniela Mata Lugo
//Fecha de entrga: 06 de septiembre de 2025
//319302149


#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"  

void Inputs(GLFWwindow* window);

static constexpr GLint COLOR = 1;
static inline void Color(const glm::vec3& c) {
    glDisableVertexAttribArray(COLOR);
    glVertexAttrib3f(COLOR, c.x, c.y, c.z);
}


const GLint WIDTH = 800, HEIGHT = 600;
float movX = 0.0f, movY = 0.0f, movZ = -12.0f;
float rot = 0.0f;

int main() {
    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "DANIELA MATA", nullptr, nullptr);
    if (!window) { std::cerr << "Failed to create GLFW window\n"; glfwTerminate(); return EXIT_FAILURE; }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) { std::cerr << "Failed to initialise GLEW\n"; return EXIT_FAILURE; }

    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    glViewport(0, 0, screenWidth, screenHeight);
    glEnable(GL_DEPTH_TEST);

    Shader ourShader("Shader/core.vs", "Shader/core.frag");

    float vertices[] = {
        // front
        -0.5f,-0.5f, 0.5f,  0.5f,-0.5f, 0.5f,  0.5f, 0.5f, 0.5f,
         0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f,-0.5f, 0.5f,
         // back
         -0.5f,-0.5f,-0.5f,  0.5f,-0.5f,-0.5f,  0.5f, 0.5f,-0.5f,
          0.5f, 0.5f,-0.5f, -0.5f, 0.5f,-0.5f, -0.5f,-0.5f,-0.5f,
          // right
           0.5f,-0.5f, 0.5f,  0.5f,-0.5f,-0.5f,  0.5f, 0.5f,-0.5f,
           0.5f, 0.5f,-0.5f,  0.5f, 0.5f, 0.5f,  0.5f,-0.5f, 0.5f,
           // left
           -0.5f, 0.5f, 0.5f, -0.5f, 0.5f,-0.5f, -0.5f,-0.5f,-0.5f,
           -0.5f,-0.5f,-0.5f, -0.5f,-0.5f, 0.5f, -0.5f, 0.5f, 0.5f,
           // bottom
           -0.5f,-0.5f,-0.5f,  0.5f,-0.5f,-0.5f,  0.5f,-0.5f, 0.5f,
            0.5f,-0.5f, 0.5f, -0.5f,-0.5f, 0.5f, -0.5f,-0.5f,-0.5f,
            // top
            -0.5f, 0.5f,-0.5f,  0.5f, 0.5f,-0.5f,  0.5f, 0.5f, 0.5f,
             0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f,-0.5f
    };

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f),
        (float)screenWidth / (float)screenHeight,
        0.1f, 100.0f);

    // Colores
    const glm::vec3 AMARILLO = { 1.00f, 1.00f, 0.52f };
    const glm::vec3 NEGRO = { 0.05f, 0.05f, 0.05f };
    const glm::vec3 ROJO = { 0.95f, 0.15f, 0.15f };
    const glm::vec3 CAFE = { 0.55f, 0.35f, 0.15f };
    const glm::vec3 BLANCO = { 1.00f, 1.00f, 1.00f };
    const glm::vec3 ROSA = { 1.00f, 0.65f, 0.75f };
    const glm::vec3 NARANJA = { 1.00f, 0.66f, 0.20f };

    while (!glfwWindowShouldClose(window)) {
        Inputs(window);
        glfwPollEvents();

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.Use();

        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(movX, movY, movZ));
        view = glm::rotate(view, glm::radians(rot), glm::vec3(0, 1, 0));

        GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
        GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(VAO);

        glm::mat4 model;

        // CUERPO 
        Color(AMARILLO);
        model = glm::mat4(1.0f);
        model = glm::translate(model, { 0.0f, 0.8f, 0.0f });
        model = glm::rotate(model, glm::radians(90.0f), { 0,1,0 });
        model = glm::scale(model, { 3.0f, 2.8f, 3.1f });
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // PATAS
        Color(AMARILLO);
        model = glm::mat4(1.0f);
        model = glm::translate(model, { 0.6f,-0.8f, 1.0f });
        model = glm::rotate(model, glm::radians(90.0f), { 0,1,0 });
        model = glm::scale(model, { 0.7f,0.7f,0.7f });
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, { -0.5f,-0.8f, 1.0f });
        model = glm::rotate(model, glm::radians(90.0f), { 0,1,0 });
        model = glm::scale(model, { 0.7f,0.7f,0.7f });
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, { 0.6f,-0.8f,-1.0f });
        model = glm::rotate(model, glm::radians(90.0f), { 0,1,0 });
        model = glm::scale(model, { 0.7f, 0.7f, 0.7f });
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, { -0.5f,-0.8f,-1.0f });
        model = glm::rotate(model, glm::radians(90.0f), { 0,1,0 });
        model = glm::scale(model, { 0.7f, 0.7f, 0.7f });
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // OREJAS 
        Color(AMARILLO);
        model = glm::mat4(1.0f);
        model = glm::translate(model, { 0.7f, 2.4f, 0.9f });
        model = glm::rotate(model, glm::radians(180.0f), { 0,1,0 });
        model = glm::scale(model, { 0.7f, 1.9f, 0.3f });
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, { -0.7f, 2.4f, 0.9f });
        model = glm::rotate(model, glm::radians(180.0f), { 0,1,0 });
        model = glm::scale(model, { 0.7f, 1.9f, 0.3f });
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // PUNTAS 
        Color(NEGRO);
        model = glm::mat4(1.0f);
        model = glm::translate(model, { 0.7f, 3.5f, 0.9f });
        model = glm::scale(model, { 0.7f, 0.4f, 0.3f });
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, { -0.7f, 3.5f, 0.9f });
        model = glm::scale(model, { 0.7f, 0.4f, 0.3f });
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // MEJILLAS 
        Color(ROJO);
        model = glm::mat4(1.0f);
        model = glm::translate(model, { 1.0f, 0.3f, 1.6f });
        model = glm::scale(model, { 0.7f, 0.7f, 0.3f });
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, { -1.0f, 0.3f, 1.6f });
        model = glm::scale(model, { 0.7f, 0.7f, 0.3f });
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // OJOS 
        Color(NEGRO);
        model = glm::mat4(1.0f);
        model = glm::translate(model, { 0.6f, 1.2f,1.6f });
        model = glm::scale(model, { 0.4f, 0.4f, 0.2f });
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, { -0.6f, 1.2f, 1.6f });
        model = glm::scale(model, { 0.4f, 0.4f, 0.2f });
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        Color(BLANCO);
        model = glm::mat4(1.0f);
        model = glm::translate(model, { 0.6f, 1.31f, 1.7f });
        model = glm::scale(model, { 0.15f, 0.15f, 0.15f });
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, { -0.6f, 1.31f, 1.7f });
        model = glm::scale(model, { 0.15f, 0.15f, 0.15f });
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // NARIZ 
        Color(NEGRO);
        model = glm::mat4(1.0f);
        model = glm::translate(model, { 0.0f, 1.0f, 1.6f });
        model = glm::scale(model, { 0.25f, 0.15f, 0.15f });
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // BOCA
        Color(ROSA);
        model = glm::mat4(1.0f);
        model = glm::translate(model, { 0.0f, 0.3f, 1.7f });
        model = glm::scale(model, { 0.6f, 0.3f, 0.15f });
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        Color(NARANJA);
        model = glm::mat4(1.0f);
        model = glm::translate(model, { 0.0f, 0.39f, 1.6f });
        model = glm::scale(model, { 0.7f, 0.5f, 0.15f });
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // RAYAS 
        Color(CAFE);
        model = glm::mat4(1.0f);
        model = glm::translate(model, { 0.0f, 2.2f, 0.2f });
        model = glm::rotate(model, glm::radians(90.0f), { 1,0,0 });
        model = glm::rotate(model, glm::radians(90.0f), { 0,0,1 });
        model = glm::scale(model, { 0.7f, 3.2f, 0.15f });
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, { 0.0f, 2.2f, -0.7f });
        model = glm::rotate(model, glm::radians(90.0f), { 1,0,0 });
        model = glm::rotate(model, glm::radians(90.0f), { 0,0,1 });
        model = glm::scale(model, { 0.7f, 3.2f, 0.15f });
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, { 1.5f, 1.5f, -0.7f });
        model = glm::rotate(model, glm::radians(90.0f), { 0,1,0 });
        model = glm::scale(model, { 0.7f, 1.5f, 0.15f });
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, { 1.5f, 1.5f,0.2f });
        model = glm::rotate(model, glm::radians(90.0f), { 0,1,0 });
        model = glm::scale(model, { 0.7f, 1.5f, 0.15f });
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, { -1.5f, 1.5f, -0.7f });
        model = glm::rotate(model, glm::radians(90.0f), { 0,1,0 });
        model = glm::scale(model, { 0.7f, 1.5f, 0.15f });
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, { -1.5f, 1.5f,0.2f });
        model = glm::rotate(model, glm::radians(90.0f), { 0,1,0 });
        model = glm::scale(model, { 0.7f, 1.5f, 0.15f });
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // COLA
        Color(AMARILLO);
        model = glm::mat4(1.0f);
        model = glm::translate(model, { 1.15f, 2.35f, -1.85f });
        model = glm::scale(model, { 1.2f, 0.7f, 0.15f });
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, { 1.35f, 2.35f, -1.98f });
        model = glm::scale(model, { 0.7f, 0.7f, 0.15f });
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, { 1.35f, 3.0f, -1.98f });
        model = glm::scale(model, { 0.7f, 0.7f, 0.15f });
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, { 1.95f, 3.0f, -1.98f });
        model = glm::scale(model, { 1.2f, 0.7f, 0.15f });
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        Color(CAFE);
        model = glm::mat4(1.0f);
        model = glm::translate(model, { 0.3f, 0.8f,-1.6f });
        model = glm::scale(model, { 0.7f, 0.7f, 0.15f });
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, { 0.9f, 1.15f,-1.75f });
        model = glm::scale(model, { 0.7f, 1.4f, 0.15f });
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, { 0.9f, 1.65f,-1.85f });
        model = glm::scale(model, { 0.7f, 0.7f, 0.15f });
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);


        glBindVertexArray(0);
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return EXIT_SUCCESS;
}

void Inputs(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)  movX += 0.01f;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)  movX -= 0.01f;
    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)  movY += 0.01f;
    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) movY -= 0.01f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)  movZ -= 0.01f;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)  movZ += 0.01f;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)  rot += 0.1f;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)  rot -= 0.1f;
}