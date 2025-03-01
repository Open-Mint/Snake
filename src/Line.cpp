#include "Line.h"

Line::Line(int gridWidth, int gridHeight, int cellSize)
: gridWidth(gridWidth), gridHeight(gridHeight), cellSize(cellSize)
{
    float vertices[] = {
           -1.0f,   1.0f,  0.0f,
            1.0f,   1.0f,  0.0f
    };
   
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
}

Line::~Line(){}

void Line::render(Shader& shader)
{
    shader.use();
    glm::mat4 model = glm::mat4(1.0f);
    // For reasons unknow to me right now, line renders vertically instead of horizontally, therefore it needs to be rotated
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(gridHeight * cellSize, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(1.0f, 8000.0f, 1));
    shader.setMat4("model", model);
    
    glEnableVertexAttribArray(VAO);
    glDrawArrays(GL_LINES, 0, 2);
}
