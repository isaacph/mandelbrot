#ifndef SRC_GAME_H_INCLUDED
#define SRC_GAME_H_INCLUDED
#include <iostream>
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <fstream>
#include <stdexcept>
#include <string_view>
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "graphics/graphics.h"
#include "graphics/simple.h"
#include "graphics/texture.h"
#include "util.h"
#include <span>
#define MY_PI 3.1415926535979323f
#include <memory>
#include <cmath>
#include <functional>
#include <set>
#include "mandelbrot.h"

const float GRAV_ACCEL= 1.0f;
const float MAX_FALL= 10.0f;
const float JUMP_INIT_VELOCITY= -5.0f;
const float VERT_FRICTION= 1.0f;
const float VERT_ACCEL=2.0f;

glm::vec2 toImageCoordinates(glm::vec2 coord, float scale, float aspectRatio);
glm::vec2 toScreenCoordinates(glm::vec2 coord, float scale, float aspectRatio);

class Game {
public:
    void run();
    void move(float &x, float &y, float deltaf, float speed);
    void onResize(int width, int height);
    void onKey(int key, int scancode, int action, int mods);
    void onGLDebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message);
    void onMouse(int button, int action, int mods);
private:
    GLFWwindow* window;
    int windowWidth = 0, windowHeight = 0;
    glm::mat4 proj;
    glm::vec2 position;
    float scale;
    float aspectRatio;
};

#endif
