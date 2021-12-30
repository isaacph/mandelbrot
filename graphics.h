#ifndef _GRAPHICS_H
#define _GRAPHICS_H
#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <fstream>
#include <stdexcept>
#include <string_view>
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "util.h"

const std::string VERTEX_SHADER_SUFFIX = "_v.glsl";
const std::string FRAGMENT_SHADER_SUFFIX = "_f.glsl";

int readShader(const std::string_view& file);
void checkProgram(int program);
GLuint makeNearestTexture(const std::string_view& file);

enum ATTRIB_TYPE : GLint {
    ATTRIB_POSITION = 0,
    ATTRIB_TEXTURE = 1,
    ATTRIB_NORMAL = 2
};

enum UNIFORM_TYPE : GLint {
    UNIFORM_MATRIX, UNIFORM_COLOR, UNIFORM_SAMPLER
};

#endif
