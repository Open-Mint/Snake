#ifndef FOOD_H
#define FOOD_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "RandomNumberGenerator.h"

#include "Shader.h"

class Food {
public:
    Food(int gridWidth, int gridHeight, int cellSize);
    ~Food();
    void render(const Shader &shader);
    void respawn();
    glm::ivec2 getPosition() const;
private:
    glm::ivec2 position;
    int gridWidth, gridHeight, cellSize;
    unsigned int VAO, VBO, EBO;
};

#endif
