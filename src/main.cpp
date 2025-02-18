#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void render_game(GLFWwindow* window);
void render_main_menu(GLFWwindow* window);
void render_highscore(GLFWwindow* window);

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
    
    // render loop
    while(!glfwWindowShouldClose(window)) {
        if(window_state == STATE::MAIN_MENU) {
            render_main_menu(window);    
        }
        else
        if(window_state == STATE::GAME) {
            render_game(window);
        }
        else
        if(window_state == STATE::HIGHSCORE) {
            render_highscore(window);
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

void render_game(GLFWwindow* window) {
    processInput(window);

    glClearColor(0.4f, 0.6f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void render_main_menu(GLFWwindow* window) {
    processInput(window);

    glClearColor(0.0f, 0.8f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void render_highscore(GLFWwindow* window) {
    processInput(window);

    glClearColor(0.9f, 0.2f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);
    glfwPollEvents();
}
