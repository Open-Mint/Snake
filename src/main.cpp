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
#include "Line.h"
#include "Rect.h"

#include <sstream>
#include <iomanip>

#include <fstream>
#include <algorithm>
#include <iterator>
#include <set>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void render_game(GLFWwindow* window, Shader &shader, Shader &snake_shader, Text &textRenderer, Snake &snake, Food& food, Line& line, Shader& line_shader, Rect& rect, Shader& rect_shader);
void render_main_menu(GLFWwindow* window, Shader &shader, Text &textRenderer);
void render_highscore(GLFWwindow* window, Shader &shader, Text &textRenderer);
void handleGameInput(GLFWwindow* window);
void handleMainMenuInput(GLFWwindow* window);
void handleHighscoreInput(GLFWwindow* window);
void handleYourHighscoreInput(GLFWwindow* window);
void render_yourHighscore(GLFWwindow* window, Shader& shader, Text& textRenderer);
void save_highscore(int score, std::fstream &File);
void drawHighscore(Shader& shader, Text& textRenderer);

const GLuint SCREEN_WIDTH = 800;
const GLuint SCREEN_HEIGHT = 800;

enum class STATE {
    MAIN_MENU,
    HIGHSCORE,
    GAME,
    YOUR_HIGHSCORE
};

STATE window_state = STATE::MAIN_MENU;
bool isEscapeKeyPressedLastFrame = false;

float speed = 0.016f;
const int GRID_CELL_SIZE = 32;
const int GRID_WIDTH = 25;
const int GRID_HEIGHT = 22;

int count = 0;
bool isFirstTime = true;
bool canWorkOnFile = false;
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
    Shader line_shader("../shader/line_shader.vs", "../shader/line_shader.fs");
    Shader rect_shader("../shader/rect_shader.vs", "../shader/rect_shader.fs");
    Text textRenderer("../resources/fonts/Nasalization Rg.otf", SCREEN_WIDTH, SCREEN_HEIGHT);
    Snake snake(window, GRID_WIDTH, GRID_HEIGHT, GRID_CELL_SIZE);
    Food food(GRID_WIDTH, GRID_HEIGHT, GRID_CELL_SIZE, snake.getSnake());
    food.initOpenGL();
    Line line(GRID_WIDTH, GRID_HEIGHT, GRID_CELL_SIZE);
    Rect rect;

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCREEN_WIDTH), 0.0f, static_cast<float>(SCREEN_HEIGHT));
    shader.use();
    shader.setMat4("projection", projection);
    
    snake_shader.use();
    snake_shader.setMat4("projection", projection);

    line_shader.use();
    line_shader.setMat4("projection", projection);

    rect_shader.use();
    rect_shader.setMat4("projection", projection);

    std::fstream file("../highscore/highscore.txt", std::ios::in | std::ios::out | std::ios::app);
    std::fstream new_file;
    if (file.is_open())
    {
        canWorkOnFile = true;
        file.close();
    }
    else {
        new_file = std::fstream("../highscore/highscore.txt", std::ios::in | std::ios::out | std::ios::app);
        canWorkOnFile = true;
    }
    
    // render loop
    while(!glfwWindowShouldClose(window)) {
        if(window_state == STATE::MAIN_MENU) {
            render_main_menu(window, shader, textRenderer);    
        }
        else
        if(window_state == STATE::GAME) {
            render_game(window, shader, snake_shader, textRenderer, snake, food, line, line_shader, rect, rect_shader);
        }
        else
        if(window_state == STATE::HIGHSCORE) {
            render_highscore(window, shader, textRenderer);
        }
        else 
        if(window_state == STATE::YOUR_HIGHSCORE){
            render_yourHighscore(window, shader, textRenderer);
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
        case STATE::YOUR_HIGHSCORE:
            handleYourHighscoreInput(window);
            break;
    }    
}

void handleYourHighscoreInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && !isEscapeKeyPressedLastFrame) {
        window_state = STATE::MAIN_MENU;
        isEscapeKeyPressedLastFrame = true;
    } else if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE) {
        isEscapeKeyPressedLastFrame = false;
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

void render_yourHighscore(GLFWwindow* window, Shader& shader, Text& textRenderer)
{
    processInput(window);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    std::ostringstream your_highscore;
    your_highscore << "Your highscore is: " << count;
    textRenderer.renderText(shader, your_highscore.str(), SCREEN_WIDTH / 2 - 300, SCREEN_HEIGHT / 2 + 100, 1.0f, glm::vec3(0.827f, 0.827f, 0.827f));
    textRenderer.renderText(shader, "Press ESC for main menu", SCREEN_WIDTH / 2 - 300, SCREEN_HEIGHT / 2, 1.0f, glm::vec3(0.827f, 0.827f, 0.827f));
    
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void render_game(GLFWwindow* window, Shader &shader, Shader &snake_shader, Text &textRenderer, Snake &snake, Food& food, Line& line, Shader& line_shader, Rect& rect, Shader& rect_shader) {
    if (isFirstTime == true)
    {
        count = 0;
        snake.setState(SNAKE_STATE::LEFT);
        speed = 0.016f;
        isFirstTime = false;
    }
    processInput(window);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    std::ostringstream count_string;
    count_string << std::setw(4) << std::setfill('0') << count; 
    textRenderer.renderText(shader, count_string.str(), SCREEN_WIDTH - 150, SCREEN_HEIGHT - 60, 1.0f, glm::vec3(0.827f, 0.827f, 0.827f)); // keeps track of highscore on screen
    snake_shader.use();

    if (snake.getSnake().front() == food.getPosition())
    {
        snake.addSegment();
        food.respawn();
        count++;
        speed += 0.001f;
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
        isFirstTime = true;
        window_state = STATE::YOUR_HIGHSCORE;
        if (canWorkOnFile && count != 0)
        {
            std::fstream new_file("../highscore/highscore.txt", std::ios::in | std::ios::out | std::ios::app);
            save_highscore(count, new_file);
        }
    }
    auto front = snake.getSnake().front();
    for (int i = snake.getSnake().size() - 1; i > 0; --i)
    {
        if (front.x == snake.getSnake()[i].x && front.y == snake.getSnake()[i].y)
        {
            snake.reset();
            isFirstTime = true;
            window_state = STATE::YOUR_HIGHSCORE;
            if (canWorkOnFile && count != 0)
            {
                std::fstream new_file("../highscore/highscore.txt", std::ios::in | std::ios::out | std::ios::app);
                save_highscore(count, new_file);
            }
            window_state = STATE::YOUR_HIGHSCORE;
        }
    }
    snake.render(snake_shader, speed);
    food.render(snake_shader);
    line.render(line_shader);
    rect.render(rect_shader, GRID_HEIGHT, GRID_CELL_SIZE);

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
    drawHighscore(shader, textRenderer);
    glfwSwapBuffers(window);
    glfwPollEvents();
}

// Save up to three highscores and if the current score gets bigger or equal to any of the current highscores place it where it belongs
void save_highscore(int score, std::fstream &File) 
{
    // Reads contents of the file
    std::vector<char> highscoreContent((std::istreambuf_iterator<char>(File)), std::istreambuf_iterator<char>());
    
    std::vector<std::string> numbers;
    
    // Puts data into the ostringstream in a way so that we can have more than one digit numbers
    std::ostringstream s;
    for (int i = 0; i < highscoreContent.size(); i++)
    {
        if (highscoreContent[i] == '\n')
        {
            numbers.emplace_back(s.str());
            s.str("");
        }
        else {
            s << highscoreContent[i];     
        }
    }
    
    // Checking to see if there are no highscores yet
    if (numbers.empty())
    {
        numbers.push_back(std::to_string(score));
    }
    else
    {
        bool isAdded = false;

        // Insert the score in the appropriate position
        for (int i = 0; i < numbers.size(); ++i)
        {
            if (score > std::stoi(numbers[i]))
            {
                numbers.insert(numbers.begin() + i, std::to_string(score));
                isAdded = true;
                break;
            }
        }
        
        // If score was not added add it anyways
        if (!isAdded)
        {
            numbers.push_back(std::to_string(score));
        }
    }

    // Ensure we only have 3 high scores
    if (numbers.size() > 3)
    {
        numbers.pop_back();
    }

    // Write the updated scores back to the file
    File.close();
    File.open("../highscore/highscore.txt", std::ios::out | std::ios::trunc);
    for (const auto& num : numbers)
    {
        File << num << "\n";
    }
}

void drawHighscore(Shader& shader, Text& textRenderer)
{
    float spacing = 150.0f;
    std::ifstream file("../highscore/highscore.txt");
    
    if (file.good()) {
        std::vector<std::string> numbers;
        std::string line;
        
        while (std::getline(file, line)) {
            numbers.push_back(line);
        }
        
        for (const auto& n : numbers)
        {
            textRenderer.renderText(shader, n, SCREEN_WIDTH / 2 - 48, SCREEN_HEIGHT - spacing, 1.0f, glm::vec3(0.827f, 0.827f, 0.827f));
            spacing += 150.0f;
        }
    }
    file.close();
}
