#version 330 core
out vec4 fragColor;

in vec2 TexCoords;
uniform sampler2D screenTexture;

void main() {
    vec4 texColor = texture(screenTexture, vec2(1. - TexCoords.x, TexCoords.y));
    fragColor = texColor;
}