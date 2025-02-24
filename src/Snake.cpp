#include "Snake.h"


Snake::Snake(GLFWwindow* window)
: window(window), snake_state(SNAKE_STATE::LEFT), moveTimer(0.0f), moveInterval(0.21f)
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

    
    snake.push_back(glm::vec2(-0.21f, 0.0f));
    snake.push_back(glm::vec2(0.0f, 0.0f));
    snake.push_back(glm::vec2(0.21f, 0.0f));
    
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

void Snake::eat() {}
void Snake::addSegment() {}
glm::vec2 Snake::getPosition() const { return position; }

void Snake::moveBody(float dx, float dy) {
    for (int i = snake.size() - 1; i > 0; --i) {
        snake[i] = snake[i-1];
    }
    snake.front() = glm::vec2(snake.front().x + dx, snake.front().y + dy);
}

void Snake::render(const Shader& shader, float dt) {
    moveUp();
    moveLeft();
    moveRight();
    moveDown();

    moveTimer += 0.016f;
    if (moveTimer >= moveInterval) {    
        moveTimer = 0.0f;
        switch(snake_state)
        {
            case SNAKE_STATE::UP:
            moveBody(0, 0.21);
            break;
            case SNAKE_STATE::DOWN:
            moveBody(0, -0.21);
            break;
            case SNAKE_STATE::LEFT:
            moveBody(-0.21, 0);
            break;
            case SNAKE_STATE::RIGHT:
            moveBody(0.21, 0);
            break;
        }        
    }
    for (auto& segment : snake) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.25, 0.25, 0.25)); // scales the snake by 1/4
        model = glm::translate(model, glm::vec3(segment.x, segment.y, 0.0f)); // moves the snake accordingly

        shader.setMat4("model", model);
        
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
}
