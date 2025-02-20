#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Text.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void render_game(GLFWwindow* window, Shader &shader, Text &textRenderer);
void render_main_menu(GLFWwindow* window, Shader &shader, Text &textRenderer);
void render_highscore(GLFWwindow* window, Shader &shader, Text &textRenderer);

const GLuint SCREEN_WIDTH = 800;
const GLuint SCREEN_HEIGHT = 600;

enum class STATE {
    MAIN_MENU,
    HIGHSCORE,
    GAME
};

STATE window_state = STATE::MAIN_MENU;

int main() {
    // setting basic OpenGL functionalities
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  
    
    // Create window
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    glEnable(GL_BLEND); // enables blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Shader shader("../shader/vertex.vs", "../shader/fragment.fs");
    Text textRenderer("../resources/fonts/Nasalization Rg.otf", SCREEN_WIDTH, SCREEN_HEIGHT);
 
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCREEN_WIDTH), 0.0f, static_cast<float>(SCREEN_HEIGHT));
    shader.use();
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    // render loop
    while(!glfwWindowShouldClose(window)) {
        if(window_state == STATE::MAIN_MENU) {
            render_main_menu(window, shader, textRenderer);    
        }
        else
        if(window_state == STATE::GAME) {
            render_game(window, shader, textRenderer);
        }
        else
        if(window_state == STATE::HIGHSCORE) {
            render_highscore(window, shader, textRenderer);
        }
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    // press escape to exit
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && window_state == STATE::GAME) {
        glfwSetWindowShouldClose(window, true);
    }
    else 
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        window_state = STATE::MAIN_MENU;
    }
    // press enter to play
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS && window_state == STATE::MAIN_MENU) {
        window_state = STATE::GAME;
    }
    // press h to see highscores
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS && window_state == STATE::MAIN_MENU) {
        window_state = STATE::HIGHSCORE;
    }
}

void render_game(GLFWwindow* window, Shader &shader, Text &textRenderer) {
    processInput(window);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void render_main_menu(GLFWwindow* window, Shader &shader, Text &textRenderer) {
    processInput(window);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    textRenderer.renderText(shader, "Play - Press Enter", SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2 + 100, 1.0f, glm::vec3(0.827f, 0.827f, 0.827f));
    textRenderer.renderText(shader, "Highscore - Press H", SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2, 1.0f, glm::vec3(0.827f, 0.827f, 0.827f));
    textRenderer.renderText(shader, "Quit - Press ESC", SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2 - 100, 1.0f, glm::vec3(0.827f, 0.827f, 0.827f));
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void render_highscore(GLFWwindow* window, Shader &shader, Text &textRenderer) {
    processInput(window);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    textRenderer.renderText(shader, "Highscores", SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT - 48, 1.0f, glm::vec3(0.827f, 0.827f, 0.827f));
    glfwSwapBuffers(window);
    glfwPollEvents();
}
