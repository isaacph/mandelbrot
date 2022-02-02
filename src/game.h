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

const float GRAV_ACCEL= 1.0f;
const float MAX_FALL= 10.0f;
const float JUMP_INIT_VELOCITY= -5.0f;
const float VERT_FRICTION= 1.0f;
const float VERT_ACCEL=2.0f;

class Camera {
public:
    void onResize(int windowWidth, int windowHeight);
    void center(float worldX, float worldY);
    void zoom(float worldTilesPerScreen);
    glm::mat4 getView() const;
    glm::vec2 getCenter() const;
    glm::vec2 toWorldCoordinate(glm::vec2 screenCoordinate) const;
    glm::vec2 toScreenCoordinate(glm::vec2 worldCoordinate) const;
private:
    void update();
    int windowWidth, windowHeight;
    glm::vec2 centerPos;
    float zoomMagnitude;
    glm::mat4 view, invView;
};

class Game {
public:
    void run();
    void move(float &x, float &y, float deltaf, float speed);
    void onResize(int width, int height);
    void onKey(int key, int scancode, int action, int mods);
    void onGLDebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message);
private:
    GLFWwindow* window;
    int windowWidth = 0, windowHeight = 0;
    glm::mat4 proj;
    Camera camera;
};

#endif
