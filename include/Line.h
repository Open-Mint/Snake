#ifndef LINE_H
#define LINE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

class Line
{
public:
    Line(int gridWidth, int gridHeight, int cellSize);
    ~Line();
    void render(Shader& shader);
private:
    int gridWidth, gridHeight, cellSize;
    unsigned int VAO, VBO;
};

#endif
