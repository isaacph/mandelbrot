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
#include "grid.h"
#include <memory>

class Box {
public:
    glm::vec2 position;
    glm::vec2 scale;
};

glm::mat4 toMatrix(Box box);

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
};

class Player {
public:
    Player();
    Box hitbox;
};

class GridPos {
public:
    int x, y;
    inline bool operator<(const GridPos& other) const {
        return x == other.x ? y < other.y : x < other.x;
    }
};

class GridManager {
public:
    BlockType& at(int worldx, int worldy);
    std::map<GridPos, Grid> grids;
};

class World {
public:
    Player player;
    GridManager gridManager;
};

#endif
