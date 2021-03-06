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
    glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods){
        ((Game*) glfwGetWindowUserPointer(window))->onMouse(button, action, mods);
    });

    GLuint unusedIds = 0;
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &unusedIds, GL_TRUE);
    glDebugMessageCallback(debugGLMessage, (const void*) this);
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
    glfwSwapInterval(1);

    {
        SimpleRender simpleRender;
        TextureRender textureRender;
        Mandelbrot mandelbrot;
        float x=0.0f, y=0.0f, gx=200.0f;
        scale = 1.0f;

        double currentTime = glfwGetTime();
        double lastTime = currentTime;
        double delta = 0.0;
        double clickTime = 0.0;
        bool clicking = false;
        glm::vec2 clickOrigin;
        glm::vec2 mousePrev;

        while (!glfwWindowShouldClose(window)) {
            currentTime = glfwGetTime();
            delta = currentTime - lastTime;
            lastTime = currentTime;
            int er = glGetError();
            if (er != 0) {
                std::cerr << er << std::endl;
            }

            double mx, my;
            glfwGetCursorPos(window, &mx, &my);
            glm::vec2 mousePos = {mx, windowHeight - my};

            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
                if (!clicking) {
                    clickTime = 0.0f;
                    clickOrigin = mousePos;
                } else {
                    clickTime += delta;
                    glm::vec2 moveImage = mousePos - mousePrev;
                    // move image
                    position -= moveImage / std::min<float>(windowWidth, windowHeight) * scale * 4.0f;
                }
            } else {
                if (clicking) {
                    glm::vec2 diff = mousePos - clickOrigin;
                    float dist = diff.length();
                    if (clickTime < 0.2 && dist < 5) {
                        // zoom in
                        scale /= 2;
                    }
                    clickTime = 0.0f;
                }
            }
            clicking = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    
            // convert mouse to [0, 1] scale

            mousePrev = mousePos;

            //start rendering
            glClear(GL_COLOR_BUFFER_BIT);

            mandelbrot.render(glm::mat4(1.0f), position, scale, aspectRatio);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    glfwTerminate();
}

void Game::onResize(int width, int height) {
    glViewport(0, 0, width, height);
    windowWidth = width;
    windowHeight = height;
    proj = glm::ortho<float>(0, width, height, 0, 0, 1);
    aspectRatio = (float) height / (float) width;
}

void Game::onKey(int key, int scancode, int action, int mods) {
    if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) {
        scale *= 2.0;
    }
}

void Game::onMouse(int button, int action, int mods) {
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
