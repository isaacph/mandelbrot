#include "simple.h"

SimpleRender::SimpleRender() {
    GLint vshader = readShader("res/simple_v.glsl");
    GLint fshader = readShader("res/simple_f.glsl");
    GLint program = glCreateProgram();
    glAttachShader(program, vshader);
    glAttachShader(program, fshader);
    glBindAttribLocation(program, ATTRIB_POSITION, "position");
    glLinkProgram(program);
    checkProgram(program);
    uniforms.insert({UNIFORM_MATRIX, glGetUniformLocation(program, "matrix")});
    uniforms.insert({UNIFORM_COLOR, glGetUniformLocation(program, "color")});

    glDeleteShader(vshader);
    glDeleteShader(fshader);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLfloat data[] = {
        -0.5f, -0.5f,
        -0.5f, +0.5f,
        +0.5f, +0.5f,
        +0.5f, +0.5f,
        +0.5f, -0.5f,
        -0.5f, -0.5f,
    };
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(ATTRIB_POSITION);
    glVertexAttribPointer(ATTRIB_POSITION, 2, GL_FLOAT, false, 2 * sizeof(GLfloat), (void*) 0);

    shared = std::shared_ptr<Shared>(new Shared());
    shared->program = program;
    shared->vao = vao;
    shared->vbo = vbo;
}

void SimpleRender::render(glm::mat4 matrix, glm::vec4 color) {
    glUseProgram(shared->program);
    glUniformMatrix4fv(uniforms[UNIFORM_MATRIX], 1, false, glm::value_ptr(matrix));
    glUniform4f(uniforms[UNIFORM_COLOR], color.x, color.y, color.z, color.w);
    glBindVertexArray(shared->vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

SimpleRender::Shared::~Shared() {
    glDeleteProgram(program);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}
