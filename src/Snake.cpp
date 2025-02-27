#include "Snake.h"

Snake::Snake(GLFWwindow* window, int gridWidth, int gridHeight, int cellSize)
: window(window), snake_state(SNAKE_STATE::LEFT), moveTimer(0.0f), moveInterval(0.21f),
gridWidth(gridWidth), gridHeight(gridHeight), cellSize(cellSize)
{
    float vertices[] = {
         0.1f,  0.1f, 0.0f,
         0.1f, -0.1f, 0.0f,
        -0.1f, -0.1f, 0.0f,
        -0.1f,  0.1f, 0.0f
    };
    unsigned int indices[] {
        0, 1, 3,
        1, 2, 3
    };

    
    snake.push_back(glm::ivec2(gridWidth/2-1, gridHeight/2));
    snake.push_back(glm::ivec2(gridWidth/2, gridHeight/2));
    snake.push_back(glm::ivec2(gridWidth/2+1, gridHeight/2));
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Snake::~Snake() {
    glfwDestroyWindow(window);
}

void Snake::moveUp() {
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && snake_state != SNAKE_STATE::DOWN) {
        snake_state = SNAKE_STATE::UP;
    }
}
void Snake::moveLeft() {
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && snake_state != SNAKE_STATE::RIGHT) {
        snake_state = SNAKE_STATE::LEFT;    
    }
}
void Snake::moveRight() {
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && snake_state != SNAKE_STATE::LEFT) {
        snake_state = SNAKE_STATE::RIGHT;    
    }
}
void Snake::moveDown() {
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && snake_state != SNAKE_STATE::UP) {
        snake_state = SNAKE_STATE::DOWN;
    }
}

std::deque<glm::ivec2> Snake::getSnake() const {
    return snake;
}

void Snake::addSegment() {
    snake.push_back(snake.back());
}

glm::vec2 Snake::getPosition() const { return position; }

void Snake::moveBody(int dx, int dy) {
    for (int i = snake.size() - 1; i > 0; --i) {
        snake[i] = snake[i-1];
    }
    snake.front() += glm::ivec2(dx, dy);
}

void Snake::setPosition(int x, int y) {
    snake.front() = glm::ivec2(x, y);
}

void Snake::reset()
{
    snake.clear();

    snake.push_back(glm::ivec2(gridWidth/2-1, gridHeight/2));
    snake.push_back(glm::ivec2(gridWidth/2, gridHeight/2));
    snake.push_back(glm::ivec2(gridWidth/2+1, gridHeight/2));

}

void Snake::render(const Shader& shader, float dt) {
    moveUp();
    moveLeft();
    moveRight();
    moveDown();

    moveTimer += dt;
    if (moveTimer >= moveInterval) {    
        moveTimer = 0.0f;
        switch(snake_state)
        {
            case SNAKE_STATE::UP:
            moveBody(0, 1);
            break;
            case SNAKE_STATE::DOWN:
            moveBody(0, -1);
            break;
            case SNAKE_STATE::LEFT:
            moveBody(-1, 0);
            break;
            case SNAKE_STATE::RIGHT:
            moveBody(1, 0);
            break;
        }        
    }
    for (auto& segment : snake) {
        float padding = 4.0f;
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(segment.x * cellSize + cellSize / 2, segment.y * cellSize + cellSize / 2, 0.0f)); // moves the snake accordingly
        model = glm::scale(model, glm::vec3((cellSize - padding) / 0.2f, (cellSize - padding) / 0.2f, 1)); // scales the snake by 1/4

        shader.setMat4("model", model);
        shader.setBool("colorState", false);


        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
}
