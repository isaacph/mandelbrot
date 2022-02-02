#include "mandelbrot.h"

Mandelbrot::Mandelbrot() {
    GLint vshader = readShader("res/texture_v.glsl");
    GLint fshader = readShader("res/mandelbrot_f.glsl");
    GLint program = glCreateProgram();
    glAttachShader(program, vshader);
    glAttachShader(program, fshader);
    glBindAttribLocation(program, ATTRIB_POSITION, "position");
    glBindAttribLocation(program, ATTRIB_TEXTURE, "texture");
    glLinkProgram(program);
    checkProgram(program);
    uniformMatrix = glGetUniformLocation(program, "matrix");
    uniformPosition = glGetUniformLocation(program, "position");
    uniformScale = glGetUniformLocation(program, "scale");
    uniformRatio = glGetUniformLocation(program, "ratio");

    glDeleteShader(vshader);
    glDeleteShader(fshader);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLfloat data[] = {
        -1.0f, -1.0f, 0.0f, 0.0f,
        -1.0f, +1.0f, 0.0f, 1.0f,
        +1.0f, +1.0f, 1.0f, 1.0f,

        +1.0f, +1.0f, 1.0f, 1.0f,
        +1.0f, -1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f, 0.0f
    };

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(ATTRIB_POSITION);
    glVertexAttribPointer(ATTRIB_POSITION, 2, GL_FLOAT, false, 4 * sizeof(GLfloat), (void*) 0);
    glEnableVertexAttribArray(ATTRIB_TEXTURE);
    glVertexAttribPointer(ATTRIB_TEXTURE, 2, GL_FLOAT, false, 4 * sizeof(GLfloat), (void*) (2 * sizeof(GLfloat)));

    shared = std::shared_ptr<Shared>(new Shared());
    shared->program = program;
    shared->vbo = vbo;
    shared->vao = vao;
}

void Mandelbrot::render(glm::mat4 matrix, glm::vec2 pos, float scale, float ratio) const {
    glUseProgram(shared->program);
    glUniformMatrix4fv(uniformMatrix, 1, false, glm::value_ptr(matrix));
    glUniform2f(uniformPosition, pos.x, pos.y);
    glUniform1f(uniformScale, scale);
    glUniform1f(uniformRatio, ratio);
    glBindVertexArray(shared->vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

Mandelbrot::Shared::~Shared() {
    glDeleteProgram(program);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

