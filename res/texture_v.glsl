#version 330

uniform mat4 matrix;

in vec2 position;
in vec2 texture;

out vec2 texCoord;

void main() {
    gl_Position = matrix * vec4(position, 0, 1);
    texCoord = texture;
}
