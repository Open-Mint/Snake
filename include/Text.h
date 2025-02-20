#ifndef TEXT_H
#define TEXT_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>
#include <string>
#include "Shader.h"

struct Character {
    unsigned int TextureID;
    glm::ivec2   Size;
    glm::ivec2   Bearing;
    unsigned int Advance;
};

class Text {
public:
    Text(const std::string& fontPath, unsigned int screenWidth, unsigned int screenHeight);
    
    void renderText(Shader &shader, const std::string &text, float x, float y, float scale, glm::vec3 color);

private:
    void loadFont(const std::string& fontPath);
    
    std::map<GLchar, Character> Characters;
    unsigned int VAO, VBO;
    unsigned int screenWidth, screenHeight;
};

#endif
