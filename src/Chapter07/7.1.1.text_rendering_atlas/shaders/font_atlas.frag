#version 330 core
out vec4 fragColor;

in vec2 TexCoords;

uniform sampler2D fontAtlas;

void main() {
    vec2 texCoords = vec2(TexCoords.x, 1. - TexCoords.y);
    float fontCol = texture(fontAtlas, texCoords).r; 
    fragColor = vec4(vec3(fontCol), 1.);
}