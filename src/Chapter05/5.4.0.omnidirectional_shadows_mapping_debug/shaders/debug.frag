#version 330 core

out vec4 fragColor;
in vec3 TexCoords;

uniform samplerCube tex;

void main() {
    vec3 texCoords = normalize(TexCoords);
    float texC = texture(tex, texCoords).r;
    fragColor = vec4(vec3(texC), 1.);
    // fragColor = vec4(TexCoords, 1.);
}