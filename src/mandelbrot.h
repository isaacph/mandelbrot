#ifndef _GRAPHICS_TEXBUFFER_H
#define _GRAPHICS_TEXBUFFER_H
#include "graphics/graphics.h"
#include <map>
#include <memory>

class Mandelbrot {
public:
    Mandelbrot();
    void render(glm::mat4 matrix, glm::vec2 position, float scale, float ratio) const;
private:
    // Shared is used so that Renders can be copied and still work just fine
    struct Shared {
        GLint program;
        GLuint vbo;
        GLuint vao;
        ~Shared();
    };
    std::shared_ptr<Shared> shared;
    GLint uniformMatrix;
    GLint uniformPosition;
    GLint uniformScale;
    GLint uniformRatio;
};

#endif
