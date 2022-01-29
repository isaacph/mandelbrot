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
#include <cmath>

const float GRAV_ACCEL= 1;

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
    Box hitbox;
    inline void moveUp(float velocity, float deltaf){
         hitbox.postion.y-=velocity*deltaf;
    }
    
    inline void moveDown(float velocity, float deltaf){
         hitbox.postion.y+=velocity*deltaf;
    }
    inline void moveRight(float velocity, float deltaf){
         hitbox.postion.x+=velocity*deltaf;
    }
    inline void moveLeft(float velocity, float deltaf){
         hitbox.postion.x-=velocity*deltaf;
    }
    float airtime;
};
class World {
public:
    Player player;
<<<<<<< HEAD
    std::map<GridPos, Grid> grids;
private:
    void playerPhysics();
    void gravity(Player player, double velocity)
=======
    GridManager gridManager;
>>>>>>> 6926560e024cfd7f8dd01e365b250dd4215d4f26
};

#endif
