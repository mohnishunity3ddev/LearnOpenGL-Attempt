#version 330 core
out vec4 fragColor;

in vec2 TexCoords;

void main() {
    fragColor = vec4(TexCoords, 0.0, 1.0);
}