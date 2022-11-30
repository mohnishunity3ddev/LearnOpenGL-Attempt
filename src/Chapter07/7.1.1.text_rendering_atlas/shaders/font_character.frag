#version 330 core
out vec4 fragColor;

in vec2 texCoords;

uniform sampler2D fontAtlas;

void main() {
    vec2 TexCoords = vec2(texCoords.x, texCoords.y);
    float fontCol = texture(fontAtlas, texCoords).r; 
    // fragColor = vec4(vec3(fontCol), 1.);
    fragColor = vec4(vec3(fontCol), 1.);// + vec4(TexCoords, 0., 0.);
}