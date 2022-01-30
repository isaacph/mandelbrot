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

        x = 0.0f; y= 0.0f;
        double growthRate=10.0f;
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

            glm::vec2 camPos = camera.getCenter();
            glm::vec2 playerMove = glm::vec2(0.0f, 0.0f);
            //playerMove.x += (glfwGetKey(window, GLFW_KEY_RIGHT) - glfwGetKey(window, GLFW_KEY_LEFT));
            //playerMove.y += (glfwGetKey(window, GLFW_KEY_DOWN) - glfwGetKey(window, GLFW_KEY_UP));
            //playerMove /= playerMove.length();
            //playerMove *= (delta * 5);

            // instead of moving the object the entire distance at once and then checking collision,
            // we move the object little by little, and check collision each time
            while (glm::length(playerMove) > 0 && false) {
                // get the next bit to move -> partialMove
                glm::vec2 partialMove;
                if (glm::length(playerMove) > MOVE_INTERPOLATE_DISTANCE_LIMIT) {
                    partialMove = playerMove / (playerMove.length() * MOVE_INTERPOLATE_DISTANCE_LIMIT);
                    playerMove -= partialMove;
                } else {
                    partialMove = playerMove;
                    playerMove = {0, 0};
                }

                // actually move the player a bit
                world.player.hitbox.position += partialMove;
                
                // check collision
            }

            camera.center(world.player.hitbox.position.x, world.player.hitbox.position.y);
            glm::vec2 mouseWorldPos = camera.toWorldCoordinate(mousePos);

            // draw on the grid with the mouse
            std::cout << floorInt(mouseWorldPos.x) << ", " << floorInt(mouseWorldPos.y) << std::endl;
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
                world.gridManager.set(1, floorInt(mouseWorldPos.x), floorInt(mouseWorldPos.y));
            }
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
                world.gridManager.set(air, floorInt(mouseWorldPos.x), floorInt(mouseWorldPos.y));
            }

            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
                x += deltaf;
            }

            Box playerRenderBox;
            playerRenderBox.position = world.player.hitbox.position;
            playerRenderBox.scale = {1, 2};
            glm::mat4 playerMatrix = toMatrix(playerRenderBox);
            //glBindTexture(GL_TEXTURE_2D, tex2);
            //textureRender.render(proj * camera.getView() * playerMatrix, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 0);
            simpleRender.render(proj * camera.getView() * playerMatrix, glm::vec4(1.0f));

            for (const auto& p : gridRendering) {
                GridPos pos = p.first;
                const TexturedBuffer& draw = p.second;
                Box gridBox;
                gridBox.position = {pos.x * GRID_SIZE, pos.y * GRID_SIZE};
                gridBox.scale = {GRID_SIZE, GRID_SIZE};
                glBindTexture(GL_TEXTURE_2D, tex);
                draw.render(proj * camera.getView() * toMatrix(gridBox), glm::vec4(1.0f), 0);
            }

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
