#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Rect
{
public:
    Rect();
    ~Rect();
    void render(Shader& shader, int gridHeight, int cellSize);
private:
    unsigned int VAO, VBO, EBO;
};
