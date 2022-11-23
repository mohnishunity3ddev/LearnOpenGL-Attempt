#version 330 core
out vec4 fragColor;

in vec3 TexCoords;

uniform samplerCube tex;

void main() {
    vec3 texCoords = normalize(TexCoords);
    float d01 = texture(tex, texCoords).r;
    fragColor = vec4(d01, d01, d01, 1.0);
    // fragColor = vec4(texCoords, 1.0);
}