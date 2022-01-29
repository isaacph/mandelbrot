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


void debugGLMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void* userPtr) {
    std::cout << "OpenGL Debug Message: (src: " << source << ", type: " << type << ", id: " << id << ", sev: " << severity << ", len: " << length << ", message:\n";
    std::cout << message << std::endl;
}

void debugGLFWMessage(int error, const char* desc) {
    throw std::runtime_error(std::string("GLFW runtime error ") + std::to_string(error) + std::string(desc));
}
 //hello
class Game {
public:
    void run() {

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
            GLuint tex = makeNearestTexture("res/cat.png");
            SimpleRender simpleRender;
            TextureRender textureRender;
            float x=0.0f, y=200.0f, gx=200.0f;

            double currentTime = glfwGetTime();
            double lastTime = currentTime;
            double delta = 0;
            double speed= 300.0f;

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

                move(x,y,deltaf,speed);
                if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
                    if(gx<50.0f)
                    gx=500.0f *deltaf;
                    else
                    gx-= growthRate*deltaf;
            }
         if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
                    if(gx>500.0f)
                    gx=50.0f;
                    else
                   gx+= growthRate*deltaf;
            }

                glm::mat4 matrix = glm::mat4(1.0f);
                matrix = glm::translate(matrix, glm::vec3(x, y, 0));
                matrix = glm::scale(matrix, glm::vec3(gx, gx, 0));
                //matrix = glm::rotate(matrix, 45.0f / 180.0f * MY_PI, glm::vec3(0, 0, 1));

                glBindTexture(GL_TEXTURE_2D, tex);
                textureRender.render(proj * matrix, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), 0);

                glfwSwapBuffers(window);
                glfwPollEvents();
            }
        }

        glfwTerminate();
    }
    void move(float &x, float &y, float deltaf, float speed){
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
    void onResize(int width, int height) {
        glViewport(0, 0, width, height);
        windowWidth = width;
        windowHeight = height;
        proj = glm::ortho<float>(0, width, height, 0, 0, 1);
    }

    void onKey(int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    }

    void onGLDebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message) {
        std::cout << "OpenGL Debug Message: (src: " << source << ", type: " << type << ", id: " << id << ", sev: " << severity << ", len: " << length << ", message:\n";
        std::cout << message << std::endl;
    }
private:
    GLFWwindow* window;
    int windowWidth = 0, windowHeight = 0;
    glm::mat4 proj;
};

int main() {
    try {
        Game().run();
    } catch (const std::exception& e) {
        std::cerr << "Error! Exiting program. Info: " << e.what() << std::endl;
    }
    return 0;
}
