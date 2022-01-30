#include "texbuffer.h"

TexturedBuffer::TexturedBuffer(const std::vector<GLfloat>& data) {
    GLint vshader = readShader("res/texture_v.glsl");
    GLint fshader = readShader("res/texture_f.glsl");
    GLint program = glCreateProgram();
    glAttachShader(program, vshader);
    glAttachShader(program, fshader);
    glBindAttribLocation(program, ATTRIB_POSITION, "position");
    glBindAttribLocation(program, ATTRIB_TEXTURE, "texture");
    glLinkProgram(program);
    checkProgram(program);
    uniforms.insert({UNIFORM_MATRIX, glGetUniformLocation(program, "matrix")});
    uniforms.insert({UNIFORM_SAMPLER, glGetUniformLocation(program, "sampler")});
    uniforms.insert({UNIFORM_COLOR, glGetUniformLocation(program, "color")});

    glDeleteShader(vshader);
    glDeleteShader(fshader);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), data.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(ATTRIB_POSITION);
    glVertexAttribPointer(ATTRIB_POSITION, 2, GL_FLOAT, false, 4 * sizeof(GLfloat), (void*) 0);
    glEnableVertexAttribArray(ATTRIB_TEXTURE);
    glVertexAttribPointer(ATTRIB_TEXTURE, 2, GL_FLOAT, false, 4 * sizeof(GLfloat), (void*) (2 * sizeof(GLfloat)));

    shared = std::shared_ptr<Shared>(new Shared());
    shared->program = program;
    shared->vbo = vbo;
    shared->vao = vao;
    shared->length = data.size();
}

void TexturedBuffer::rebuild(const std::vector<float> &buffer) {
    glBindVertexArray(shared->vao);
    glBindBuffer(GL_ARRAY_BUFFER, shared->vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, buffer.size() * sizeof(GLfloat), buffer.data());
    //glEnableVertexAttribArray(ATTRIB_POSITION);
    //glVertexAttribPointer(ATTRIB_POSITION, 2, GL_FLOAT, false, 4 * sizeof(GLfloat), (void*) 0);
    //glEnableVertexAttribArray(ATTRIB_TEXTURE);
    //glVertexAttribPointer(ATTRIB_TEXTURE, 2, GL_FLOAT, false, 4 * sizeof(GLfloat), (void*) (2 * sizeof(GLfloat)));
}

void TexturedBuffer::render(glm::mat4 matrix, glm::vec4 color, GLint sampler) const {
    glUseProgram(shared->program);
    glUniformMatrix4fv(uniforms.at(UNIFORM_MATRIX), 1, false, glm::value_ptr(matrix));
    glUniform4f(uniforms.at(UNIFORM_COLOR), color.x, color.y, color.z, color.w);
    glUniform1i(uniforms.at(UNIFORM_SAMPLER), sampler);
    glBindVertexArray(shared->vao);
    glDrawArrays(GL_TRIANGLES, 0, shared->length / 4);
}

TexturedBuffer::Shared::~Shared() {
    glDeleteProgram(program);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}
