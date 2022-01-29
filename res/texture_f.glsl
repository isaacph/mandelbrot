#version 330

uniform vec4 color;
uniform sampler2D sampler;

in vec2 texCoord;

out vec4 outColor;

void main() {
   outColor = texture2D(sampler, texCoord) * color;
}
