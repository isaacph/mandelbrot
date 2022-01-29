#ifndef _GRAPHICS_TEXTURE_H
#define _GRAPHICS_TEXTURE_H
#include "graphics.h"
#include <map>
#include <memory>

class GridRender {
public:
    GridRender();
    void render(glm::mat4 matrix, glm::vec4 color, GLint sampler);
private:
    // Shared is used so that Renders can be copied and still work just fine
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
