#version 330 core
out vec4 fragColor;

in vec2 TexCoords;
uniform sampler2D depthMap;

void main() {
    float depth = texture(depthMap, TexCoords).r;
    fragColor = vec4(vec3(depth), 1.0);// + vec4(TexCoords, 0, 0);
}