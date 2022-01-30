#include "game.h"
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
#include "graphics/texbuffer.h"
#include "util.h"
#include <span>
#include <box2d/box2d.h>
#define MY_PI 3.1415926535979323f

const float MOVE_INTERPOLATE_DISTANCE_LIMIT = 0.1f;

void debugGLMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void* userPtr) {
    std::cout << "OpenGL Debug Message: (src: " << source << ", type: " << type << ", id: " << id << ", sev: " << severity << ", len: " << length << ", message:\n";
    std::cout << message << std::endl;
}

void debugGLFWMessage(int error, const char* desc) {
    throw std::runtime_error(std::string("GLFW runtime error ") + std::to_string(error) + std::string(desc));
}



 //hello
void Game::run() {
    glfwSetErrorCallback(debugGLFWMessage);
    if (!glfwInit())
        throw std::runtime_error("Failed to initialize GLFW");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    window = glfwCreateWindow(800, 600, "Game Dev", NULL, NULL);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSetWindowUserPointer(window, (void*) this);
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    onResize(windowWidth, windowHeight);
    glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int w, int h) {
        ((Game*) glfwGetWindowUserPointer(window))->onResize(w, h);
    });
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods){
        ((Game*) glfwGetWindowUserPointer(window))->onKey(key, scancode, action, mods);
    });

    GLuint unusedIds = 0;
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &unusedIds, GL_TRUE);
    glDebugMessageCallback(debugGLMessage, (const void*) this);
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
    glfwSwapInterval(1);

    {
        GLuint tex = makeNearestTexture("res/tilesheet.png");
        GLuint tex2 = makeNearestTexture("res/blobfish.png");
        SimpleRender simpleRender;
        TextureRender textureRender;
        float x=0.0f, y=0.0f, gx=200.0f;

        double currentTime = glfwGetTime();
        double lastTime = currentTime;
        double delta = 0;
        double speed= 300.0f;

        World world;
        std::map<GridPos, TexturedBuffer> gridRendering;
        b2Vec2 worldGravity(0.0f, 9.8f);
        b2World box2dWorld(worldGravity);
        b2BodyDef groundBodyDef;
        groundBodyDef.position.Set(0.0f, 5.0f);
        b2Body* groundBody = box2dWorld.CreateBody(&groundBodyDef);
        b2PolygonShape b2GroundBox;
        b2GroundBox.SetAsBox(10.0f, 5.0f);
        groundBody->CreateFixture(&b2GroundBox, 0.0f);

        b2BodyDef playerBodyDef;
        playerBodyDef.type = b2_dynamicBody;
        playerBodyDef.position.Set(0.0f, -5.0f);
        b2Body* playerBody = box2dWorld.CreateBody(&playerBodyDef);
        b2PolygonShape dynamicBox;
        dynamicBox.SetAsBox(0.5f, 1.0f);
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.3f;
        playerBody->CreateFixture(&fixtureDef);

        auto gridChangeSub = world.gridManager.gridChanges.subscribe([&gridRendering](std::pair<GridPos, Grid> grid) {
            std::vector<GLfloat> testBuffer = makeTexturedBuffer(grid.second);
            auto p = gridRendering.find(grid.first);
            if (p != gridRendering.end()) {
                p->second.rebuild(testBuffer);
            } else {
                gridRendering.insert({grid.first, TexturedBuffer(testBuffer)});
            }
        });
        world.gridManager.set(1, 0, 0);

        camera.zoom(16.0f);

        double physicsTime = 0;
        int physicsFrames = 0;
        while (!glfwWindowShouldClose(window)) {
            currentTime = glfwGetTime();
            delta = currentTime - lastTime;
            lastTime = currentTime;
            float deltaf = (float) delta;
            int er = glGetError();
            if (er != 0) {
                std::cerr << er << std::endl;
            }
            glClear(GL_COLOR_BUFFER_BIT);

            double mx, my;
            glfwGetCursorPos(window, &mx, &my);
            glm::vec2 mousePos = {mx, my};

            glm::vec2 playerMove = glm::vec2(0.0f, 0.0f);
            float playerJump = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS ? -8.0f : 0.0f;
            playerMove.x += (glfwGetKey(window, GLFW_KEY_RIGHT) - glfwGetKey(window, GLFW_KEY_LEFT));
            if (playerMove.length() >= 0) {
                playerMove /= playerMove.length();
            }
            
            playerMove *= 100;
            b2Vec2 playerMoveForce(playerMove.x, playerMove.y);
            playerBody->ApplyForce(playerMoveForce, playerBody->GetPosition(), true);

            glm::vec2 mouseWorldPos = camera.toWorldCoordinate(mousePos);

            // draw on the grid with the mouse
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
                world.gridManager.set(1, floorInt(mouseWorldPos.x), floorInt(mouseWorldPos.y));
            }
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
                world.gridManager.set(air, floorInt(mouseWorldPos.x), floorInt(mouseWorldPos.y));
            }

            physicsTime += delta;
            double timeStep = 1.0 / 60.0f;
            while (physicsTime > timeStep) {
                b2Vec2 playerMoveForce(playerMove.x * timeStep, playerMove.y * timeStep);
                b2Vec2 playerJumpImpulse(0, playerJump);
                if (playerMoveForce.LengthSquared() > 0.00001f) {
                    playerBody->ApplyForce(playerMoveForce, playerBody->GetPosition(), true);
                }
                if (playerJumpImpulse.LengthSquared() > 0.00001f) {
                    playerBody->ApplyLinearImpulseToCenter(playerJumpImpulse, true);
                }

                physicsFrames++;
                box2dWorld.Step(1.0f / 60.0f, 8, 3);
                physicsTime -= 1.0 / 60.0;
            }

            b2Vec2 playerPos = playerBody->GetPosition();
            camera.center(playerPos.x, playerPos.y);
            Box playerRenderBox;
            playerRenderBox.position = {playerPos.x, playerPos.y};
            playerRenderBox.scale = {1, 2};
            glm::mat4 playerMatrix = toMatrix(playerRenderBox);
            //glBindTexture(GL_TEXTURE_2D, tex2);
            //textureRender.render(proj * camera.getView() * playerMatrix, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 0);
            simpleRender.render(proj * camera.getView() * playerMatrix, glm::vec4(1.0f));

            Box groundBox;
            b2Vec2 groundPos = groundBody->GetPosition();
            b2Vec2 groundScale = {20, 10};
            groundBox.position = {groundPos.x, groundPos.y};
            groundBox.scale = {groundScale.x, groundScale.y};
            glm::mat4 groundMatrix = toMatrix(groundBox);
            glBindTexture(GL_TEXTURE_2D, tex2);
            textureRender.render(proj * camera.getView() * groundMatrix, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 0);

            //for (const auto& p : gridRendering) {
            //    GridPos pos = p.first;
            //    const TexturedBuffer& draw = p.second;
            //    Box gridBox;
            //    gridBox.position = {pos.x * GRID_SIZE, pos.y * GRID_SIZE};
            //    gridBox.scale = {GRID_SIZE, GRID_SIZE};
            //    glBindTexture(GL_TEXTURE_2D, tex);
            //    draw.render(proj * camera.getView() * toMatrix(gridBox), glm::vec4(1.0f), 0);
            //}

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    glfwTerminate();
}

void Game::move(float &x, float &y, float deltaf, float speed){
     if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
                if(x>windowWidth)
                x=0.0f *deltaf;
                else
                x+= speed*deltaf;
        }
     if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
                if(x<0)
                x=(float)windowWidth;
                else
                x-= speed*deltaf;
        }
     if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
                if(y<0)
                y=(float)windowHeight;
                else
                y-= speed*deltaf;
        }
     if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
                if(y>windowHeight)
                y=0.0f *deltaf;
                else
                y+= speed*deltaf;
         }
}
void Game::onResize(int width, int height) {
    glViewport(0, 0, width, height);
    windowWidth = width;
    windowHeight = height;
    proj = glm::ortho<float>(0, width, height, 0, 0, 1);
    camera.onResize(width, height);
}

void Game::onKey(int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void Game::onGLDebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message) {
    std::cout << "OpenGL Debug Message: (src: " << source << ", type: " << type << ", id: " << id << ", sev: " << severity << ", len: " << length << ", message:\n";
    std::cout << message << std::endl;
}

int main() {
    try {
        Game().run();
    } catch (const std::exception& e) {
        std::cerr << "Error! Exiting program. Info: " << e.what() << std::endl;
    }
    return 0;
}

//            std::vector<std::unique_ptr<Convex>> toTestCollision;
//
//            // instead of moving the object the entire distance at once and then checking collision,
//            // we move the object little by little, and check collision each time
//            while (glm::length(playerMove) > 0) {
//                // get the next bit to move -> partialMove
//                glm::vec2 partialMove;
//                if (glm::length(playerMove) > MOVE_INTERPOLATE_DISTANCE_LIMIT) {
//                    partialMove = playerMove / (playerMove.length() * MOVE_INTERPOLATE_DISTANCE_LIMIT);
//                    playerMove -= partialMove;
//                } else {
//                    partialMove = playerMove;
//                    playerMove = {0, 0};
//                }
//
//                // actually move the player a bit
//                world.player.hitbox.position += partialMove;
//                
//                // check collision
//                // grab all the objects to collide with (in collisionTesters)
//                std::vector<Convex*> collisionTesters;
//                std::vector<std::unique_ptr<Convex>> temp;
//                for (const std::unique_ptr<Convex>& collider : toTestCollision) {
//                    collisionTesters.push_back(collider.get());
//                }
//                for (GridPos tilePos : overlappingTiles(world.player.hitbox)) {
//                    if (world.gridManager.check(tilePos.x, tilePos.y) != air) {
//                        Box tile = tileBox(tilePos.x, tilePos.y);
//                        Convex* convex = new Box(tile);
//                        temp.push_back(std::unique_ptr<Convex>(convex));
//                        collisionTesters.push_back(convex);
//                    }
//                }
//
//                int minOverlaps = std::numeric_limits<int>::max();
//                glm::vec2 minOffset = {0, 0}; // minimized offset for number of overlaps
//                for (Convex* convexp : collisionTesters) {
//                    const Convex& convex = *convexp;
//                    if (intersect(world.player.hitbox, convex)) {
//                        // make a resolution and figure out how good it is
//                        Box resolvedPlayer = world.player.hitbox;
//                        glm::vec2 resolve = {0, 0};
//                        resolve += resolveX(convex, resolvedPlayer);
//                        resolvedPlayer.position = world.player.hitbox.position + resolve;
//                        resolve += resolveY(convex, resolvedPlayer);
//                        resolvedPlayer.position = world.player.hitbox.position;
//
//                        // grab all the objects to collide with (in collisionTesters2)
//                        int overlaps = 0;
//                        std::vector<Convex*> collisionTesters2;
//                        std::vector<std::unique_ptr<Convex>> temp;
//                        for (const std::unique_ptr<Convex>& collider : toTestCollision) {
//                            collisionTesters2.push_back(collider.get());
//                        }
//                        for (GridPos tilePos : overlappingTiles(world.player.hitbox)) {
//                            if (world.gridManager.check(tilePos.x, tilePos.y) != air) {
//                                Box tile = tileBox(tilePos.x, tilePos.y);
//                                Convex* convex = new Box(tile);
//                                temp.push_back(std::unique_ptr<Convex>(convex));
//                                collisionTesters2.push_back(convex);
//                            }
//                        }
//
//                        // count overlaps
//                        for (Convex* convexp : collisionTesters2) {
//                            const Convex& convex = *convexp;
//                            if (intersect(world.player.hitbox, convex)) {
//                                ++overlaps;
//                            }
//                        }
//
//                        if (overlaps < minOverlaps) {
//                            minOverlaps = overlaps;
//                            minOffset = resolve;
//                        }
//                    }
//                }
//
//                // try to resolve a collision
//                world.player.hitbox.position += minOffset;
//            }
