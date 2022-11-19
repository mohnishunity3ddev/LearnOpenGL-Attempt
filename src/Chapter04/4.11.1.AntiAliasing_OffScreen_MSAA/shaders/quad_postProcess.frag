#version 330 core
out vec4 fragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main() {
    vec4 texColor = texture(screenTexture, TexCoords);
    float grayscale = 0.2126 * texColor.r + 0.7152 * texColor.g + 0.0722 * texColor.b;
    fragColor = vec4(vec3(grayscale), 1.0);
}