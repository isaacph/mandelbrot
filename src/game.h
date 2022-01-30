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
#include "physics.h"
#include <box2d/box2d.h>
#include <set>

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

struct BodyType {};
struct BoxBodyType : public BodyType {
    glm::vec2 scale;
};

class GameObject {
public:
    enum Type {
        PLAYER, GROUND
    };
    std::set<Type> types;
    std::string name;
    std::unique_ptr<BodyType> bodyType;
    b2Body* rigidBody;
    b2Fixture* fixture;
    GameObject(b2World* world);
    ~GameObject();
private:
    b2World* world;
};

std::unique_ptr<GameObject> makePlayer(b2World* world, glm::vec2 position);
std::unique_ptr<GameObject> makeGroundType(b2World* world, Box bodyDef);
std::vector<std::unique_ptr<GameObject>> makeGround(b2World* world, GridPos gridPos, Grid grid);

class Game : public b2ContactListener {
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
    bool foward;
    GridManager gridManager;
    b2World box2dWorld = b2World(b2Vec2(0.0f, 9.8f));
    //b2Body* groundBody;
    //b2Fixture* groundFixture;
    //b2Body* playerBody;
    //b2Fixture* playerFixture;
public:
    void BeginContact(b2Contact* contact);
     
    void EndContact(b2Contact* contact);
     
    void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
     
    void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
};

class Player {
public:
    Box hitbox;
    float x_velocity;
    float y_velocity;
    inline Player() {
        hitbox.position = {0.0f, 0.0f};
        hitbox.scale = {1.0f, 1.0f};
    }
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
