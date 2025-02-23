#include "Snake.h"


Snake::Snake(GLFWwindow* window)
: window(window), snake_state(SNAKE_STATE::UP)
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

    
    snake.push_back(glm::vec2(0.0f, 0.0));
    snake.push_back(glm::vec2(-0.21f, 0.0f));
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
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        snake_state = SNAKE_STATE::UP;
    }
}
void Snake::moveLeft() {
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        snake_state = SNAKE_STATE::LEFT;    
    }
}
void Snake::moveRight() {
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        snake_state = SNAKE_STATE::RIGHT;    
    }
}
void Snake::moveDown() {
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        snake_state = SNAKE_STATE::DOWN;
    }
}

void Snake::eat() {}
void Snake::addSegment() {}
glm::vec2 Snake::getPosition() const { return position; }

void Snake::render(const Shader& shader, float dt) {
    moveUp();
    moveLeft();
    moveRight();
    moveDown();

    for (auto& segment : snake) {
        
        switch(snake_state)
        {
            case SNAKE_STATE::UP:
            segment.y += 0.07 * dt;
            break;
            case SNAKE_STATE::DOWN:
            segment.y -= 0.07 * dt;
            break;
            case SNAKE_STATE::LEFT:
            segment.x -= 0.07 * dt;
            break;
            case SNAKE_STATE::RIGHT:
            segment.x += 0.07 * dt;
            break;
        }        

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.25, 0.25, 0.25)); // scales the snake by 1/4
        model = glm::translate(model, glm::vec3(segment.x, segment.y, 0.0f)); // moves the snake accordingly

        shader.setMat4("model", model);
        
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
}
