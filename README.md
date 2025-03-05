# Snake
A recreation of the snake game using C++ and OpenGL 3.3

## Description
Currently the project can only be run on linux. Support for other platforms will be added in the near future
## Prerequisites
Before you can build and run the game, make sure you have these installed

- CMake Version 3.10
- OpenGL 3.3
- GLFW
- GLAD
- Freetype

### Getting Started

#### Clone the repository

```bash
git clone https://github.com/Open-Mint/Snake.git
cd Snake
```

#### Build the project
```bash
mkdir highscore
mkdir build
cd build
cmake ..
make
```

#### Running the game
```bash
./snake
```
## How to play

Use UP, DOWN, LEFT and RIGHT arrow keys respectively to move the snake

## License

This project is licensed under the [MIT License](https://opensource.org/licenses/MIT).

## Attribution

This project uses code from [Shader.h and text processing by Joey de Vries](https://www.learnopengl.com) under the following [license](https://creativecommons.org/licenses/by-nc/4.0/)

Please ensure to visit the original work at [https://www.learnopengl.com](https://www.learnopengl.com).

This project uses [glfw](https://www.glfw.org/), [glm](https://glm.g-truc.net/0.9.8/index.html), [glad](https://github.com/Dav1dde/glad) that is licensed under [MIT license](https://github.com/Dav1dde/glad/blob/glad2/LICENSE)

This project uses [nasalization free font](https://www.1001fonts.com/nasalization-free-font.html)

Unit tests are written with [gtest](https://github.com/google/googletest) which is licensed under [BSD 3-Clause](https://github.com/google/googletest/blob/main/LICENSE)

The project uses [CMake](https://cmake.org/) and [Freetype](https://freetype.org/)
### Acknowledgements:
- Full Name: Joey de Vries
- Website: [https://www.learnopengl.com](https://www.learnopengl.com)
- Twitter: [https://twitter.com/JoeyDeVriez](https://twitter.com/JoeyDeVriez)
