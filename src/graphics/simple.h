#ifndef _GRAPHICS_SIMPLE_H
#define _GRAPHICS_SIMPLE_H
#include "graphics.h"
#include <map>
#include <memory>

class SimpleRender {
public:
    SimpleRender();
    void render(glm::mat4 matrix, glm::vec4 color);
private:
    struct Shared {
        GLint program;
        GLuint vbo;
        GLuint vao;
        ~Shared();
    };
    std::shared_ptr<Shared> shared;
    std::map<UNIFORM_TYPE, GLint> uniforms;
};

#endif
