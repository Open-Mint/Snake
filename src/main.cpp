#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Text.h"
#include "Snake.h"
#include "Food.h"

#include <sstream>
#include <iomanip>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void render_game(GLFWwindow* window, Shader &shader, Shader &snake_shader, Text &textRenderer, Snake &snake, Food& food);
void render_main_menu(GLFWwindow* window, Shader &shader, Text &textRenderer);
void render_highscore(GLFWwindow* window, Shader &shader, Text &textRenderer);
void handleGameInput(GLFWwindow* window);
void handleMainMenuInput(GLFWwindow* window);
void handleHighscoreInput(GLFWwindow* window);

const GLuint SCREEN_WIDTH = 800;
const GLuint SCREEN_HEIGHT = 800;

enum class STATE {
    MAIN_MENU,
    HIGHSCORE,
    GAME
};

STATE window_state = STATE::MAIN_MENU;
bool isEscapeKeyPressedLastFrame = false;

float dt, lf; // delta time and last frame

const int GRID_CELL_SIZE = 32;
const int GRID_WIDTH = 25;
const int GRID_HEIGHT = 25;

int count = 0;
bool isFirstTime = true;

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
    Shader snake_shader("../shader/snake_shader.vs", "../shader/snake_shader.fs");
    Text textRenderer("../resources/fonts/Nasalization Rg.otf", SCREEN_WIDTH, SCREEN_HEIGHT);
    Snake snake(window, GRID_WIDTH, GRID_HEIGHT, GRID_CELL_SIZE);
    Food food(GRID_WIDTH, GRID_HEIGHT, GRID_CELL_SIZE, snake.getSnake());

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCREEN_WIDTH), 0.0f, static_cast<float>(SCREEN_HEIGHT));
    shader.use();
    shader.setMat4("projection", projection);
    
    snake_shader.use();
    snake_shader.setMat4("projection", projection);
    // render loop
    while(!glfwWindowShouldClose(window)) {
        if(window_state == STATE::MAIN_MENU) {
            render_main_menu(window, shader, textRenderer);    
        }
        else
        if(window_state == STATE::GAME) {
            render_game(window, shader, snake_shader, textRenderer, snake, food);
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
    switch(window_state) {
        case STATE::MAIN_MENU:
            handleMainMenuInput(window);
            break;
        case STATE::GAME:
            handleGameInput(window);
            break;
        case STATE::HIGHSCORE:
            handleHighscoreInput(window);
            break;
    }    
}

// handles game input - makes sure that escape key flag does not go into the next iteration of the loop
void handleGameInput(GLFWwindow* window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && !isEscapeKeyPressedLastFrame) {
        window_state = STATE::MAIN_MENU;
        isEscapeKeyPressedLastFrame = true;
    } else if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE) {
        isEscapeKeyPressedLastFrame = false;
    }
}
// handles highscore input - makes sure that escape key flag does not go into the next iteration of the loop
void handleHighscoreInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && !isEscapeKeyPressedLastFrame) {
        window_state = STATE::MAIN_MENU;
        isEscapeKeyPressedLastFrame = true;
    } else if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE) {
        isEscapeKeyPressedLastFrame = false;
    }
}
// handles main menu input - makes sure that escape key flag does not go into the next iteration of the loop
void handleMainMenuInput(GLFWwindow* window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && !isEscapeKeyPressedLastFrame) {
        glfwSetWindowShouldClose(window, true);
        isEscapeKeyPressedLastFrame = true;
    } else if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE) {
        isEscapeKeyPressedLastFrame = false;
    }
    if(glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
        window_state = STATE::GAME;
    }
    if(glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
        window_state = STATE::HIGHSCORE;
    }
}
void render_game(GLFWwindow* window, Shader &shader, Shader &snake_shader, Text &textRenderer, Snake &snake, Food& food) {
    if (isFirstTime == true)
    {
        snake.setState(SNAKE_STATE::LEFT);
        isFirstTime = false;
    }
    processInput(window);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    std::ostringstream count_string;
    count_string << std::setw(4) << std::setfill('0') << count; 
    textRenderer.renderText(shader, count_string.str(), SCREEN_WIDTH - 150, SCREEN_HEIGHT - 60, 1.0f, glm::vec3(0.827f, 0.827f, 0.827f)); // keeps track of highscore on screen
    snake_shader.use();
    float cf = glfwGetTime(); // current frame with implementation of delta time
    dt = cf - lf;
    lf = cf;

    if (snake.getSnake().front() == food.getPosition())
    {
        snake.addSegment();
        food.respawn();
        count++;
    }
    if (snake.getSnake().front().x + 1 > GRID_WIDTH) // collision right ends the game
    {
        window_state = STATE::MAIN_MENU;
    }
    if (snake.getSnake().front().x < 0) // collision left ends the game
    {
        window_state = STATE::MAIN_MENU;
    }
    if (snake.getSnake().front().y + 1 > GRID_HEIGHT) // collision up ends the game
    {
        window_state = STATE::MAIN_MENU;
    }
    if (snake.getSnake().front().y < 0) // collision down ends the game
    {
        window_state = STATE::MAIN_MENU;
    }
    if (window_state == STATE::MAIN_MENU)
    {
        snake.reset();
        count = 0;
        isFirstTime = true;
    }
    auto front = snake.getSnake().front();
    for (int i = snake.getSnake().size() - 1; i > 0; --i)
    {
        if (front.x == snake.getSnake()[i].x && front.y == snake.getSnake()[i].y)
        {
            window_state = STATE::MAIN_MENU;
        }
    }
    snake.render(snake_shader, dt);
    food.render(snake_shader);

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
