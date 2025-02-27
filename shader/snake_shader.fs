#version 330 core
out vec4 color;

uniform bool colorState;

void main()
{
    // True if food(red) false if snake(grey)
    color = colorState ? vec4(1.0, 0.0, 0.0, 1.0) : vec4(0.827, 0.827, 0.827, 1.0);
}
