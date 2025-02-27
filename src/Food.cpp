#include "Food.h"

Food::Food(int gridWidth, int gridHeight, int cellSize)
: gridWidth(gridWidth), gridHeight(gridHeight), cellSize(cellSize) {
    float vertices[] = {
         0.1f,  0.1f, 0.0f,
         0.1f, -0.1f, 0.0f,
        -0.1f, -0.1f, 0.0f,
        -0.1f,  0.1f, 0.0f
    };
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };
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

    respawn();
}

Food::~Food() {}

void Food::respawn() {
    RandomNumberGenerator rngX(0, gridWidth - 1);
    RandomNumberGenerator rngY(0, gridHeight - 1);

    position.x = rngX.getNumber();
    position.y = rngY.getNumber();
}

glm::ivec2 Food::getPosition() const {
    return position;
}

void Food::render(const Shader &shader) {
    float padding = 4.0f;
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position.x * cellSize + cellSize / 2, position.y * cellSize + cellSize / 2, 0.0f));
    model = glm::scale(model, glm::vec3((cellSize - padding) / 0.2f, (cellSize - padding) / 0.2f, 1));
    
    shader.setMat4("model", model);
    shader.setBool("colorState", true);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
