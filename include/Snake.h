#ifndef SNAKE_H
#define SNAKE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <deque>
#include "Shader.h"

enum class SNAKE_STATE {
    UP,
    LEFT,
    RIGHT,
    DOWN,
    NONE
};

class Snake {
public:
    Snake(GLFWwindow* window, int gridWidth, int gridHeight, int cellSize);
    ~Snake();
    void moveUp();
    void moveLeft();
    void moveRight();
    void moveDown();

    void moveBody(int dx, int dy);
    void addSegment();
    glm::vec2 getPosition() const;
    void render(const Shader& shader, float dt);
    std::deque<glm::ivec2> getSnake() const;
    void setPosition(int x, int y);
    void reset();
    bool canChangeDirection(float currentTime);
    void setState(SNAKE_STATE state);
private:
    glm::vec2 position;
    int length;
    std::deque<glm::ivec2> snake;
    SNAKE_STATE snake_state;
    GLFWwindow* window;
    unsigned int VAO, VBO, EBO;
    float moveTimer;
    float moveInterval;
    int gridWidth, gridHeight, cellSize;
    float coolDownTime, lastDirectionChangeTime;
};

#endif
