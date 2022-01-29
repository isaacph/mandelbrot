#ifndef _GRAPHICS_TEXBUFFER_H
#define _GRAPHICS_TEXBUFFER_H
#include "graphics.h"
#include <map>
#include <memory>

class TexturedBuffer {
public:
    TexturedBuffer(const std::vector<GLfloat>& buffer);
    void render(glm::mat4 matrix, glm::vec4 color, GLint sampler);
    void rebuild(const std::vector<GLfloat>& buffer);
private:
    // Shared is used so that Renders can be copied and still work just fine
    struct Shared {
        GLint program;
        GLuint vbo;
        GLuint vao;
        size_t length;
        ~Shared();
    };
    std::shared_ptr<Shared> shared;
    std::map<UNIFORM_TYPE, GLint> uniforms;
};

#endif
