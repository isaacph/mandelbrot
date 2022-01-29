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
#include <functional>
#include "events.h"

const float GRAV_ACCEL= 1.0f;
const float MAX_FALL= 10.0f;
const float JUMP_INIT_VELOCITY= -5.0f;
const float VERT_FRICTION= 1.0f;
const float VERT_ACCEL=2.0f;

class PlayerInstruction{
public:
    bool isJump;
    bool moveRight;
    bool moveLeft;
};
class Status{
    int hp;
    bool stunned;
    bool invunreable;
};

class Box {
public:
    glm::vec2 position;
    glm::vec2 scale;
};

glm::mat4 toMatrix(Box box);

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

class Player {
public:
    Box hitbox;
    float x_velocity;
    float y_velocity;
    inline void updateHitbox(float deltaF){
         hitbox.position.y+=x_velocity * deltaF;
         hitbox.position.x+=y_velocity * deltaF;
    }
    inline void updateXVelocity(float deltaV){
        x_velocity+=deltaV;
    };
    inline void updateYVelocity(float deltaV){
        y_velocity+=deltaV;
    };
    inline void jump(){
      if(numJumps>0){
          y_velocity= JUMP_INIT_VELOCITY;
          airtime*= 0.1f;
          numJumps--;
        }  
    };
    void playerMoveLeft(float deltaF);
    void playerMoveRight(float deltaF);
    float moveAccel=5;
    float airtime;
    int numJumps;
    float maxSpeed;
};
class World {
public:
    Player player;
    GridManager gridManager;
    void worldUpdate(float deltaF, PlayerInstruction instruct);
private:
    void playerMovement(float deltaF, PlayerInstruction instruct);
    void updatePhysics(float deltaF);
    void gravity(Player player, float deltaF);
};

#endif
