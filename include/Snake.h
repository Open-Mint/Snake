#ifndef SNAKE_H
#define SNAKE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <deque>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

enum class SNAKE_STATE {
    UP,
    LEFT,
    RIGHT,
    DOWN
};

class Snake {
public:
    Snake(GLFWwindow* window);
    ~Snake();
    void moveUp();
    void moveLeft();
    void moveRight();
    void moveDown();

    void moveBody(float dx, float dy);
    void eat();
    void addSegment();
    glm::vec2 getPosition() const;
    void render(const Shader& shader, float dt);
private:
    glm::vec2 position;
    int length;
    std::deque<glm::vec2> snake;
    SNAKE_STATE snake_state;
    GLFWwindow* window;
    unsigned int VAO, VBO, EBO;
    float moveTimer;
    float moveInterval;
};

#endif
