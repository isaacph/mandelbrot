#version 330

in vec2 texCoord;

out vec4 outColor;

uniform vec2 position;
uniform float scale;
uniform float ratio;

vec2 c2 (vec2 complex) {
    return vec2(complex.x * complex.x - complex.y * complex.y, 2 * complex.x * complex.y);
}

float mag (vec2 complex) {
    return complex.x * complex.x + complex.y * complex.y;
}

vec4 color (int count) {
    return vec4(count / 100.0 * vec3(1), 1);
}

void main() {
    vec2 pos = scale * 4 * (vec2(texCoord.x, texCoord.y) - vec2(0.5));
    pos = vec2(pos.x, pos.y * ratio);
    pos += position;
    vec2 z = pos;
    vec2 c = pos;

    int i = 0;
    for (; i < 1000; ++i) {
        if (mag(c2(z)) > 4) {
            break;
        }
        vec2 zsq = c2(z);
        z = zsq + c;
    }

    outColor = color(i);
}
