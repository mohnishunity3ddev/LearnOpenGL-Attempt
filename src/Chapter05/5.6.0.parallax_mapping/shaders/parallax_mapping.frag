#version 330 core
out vec4 fragColor;

in vec2 TexCoords;

uniform sampler2D diffuseMap, normalMap, displacementMap;

void main() {
    vec3 diffuseCol = texture(diffuseMap, TexCoords).rgb;
    vec3 displacementCol = texture(displacementMap, TexCoords).rgb;
    vec3 normalCol = texture(normalMap, TexCoords).rgb;

    fragColor = vec4(diffuseCol, 1.0);
}