#version 330 core
out vec4 fragColor;

in VS_OUT {
    vec2 texCoords;
} fs_in;

uniform sampler2D texture_diffuse1;

void main() {
    fragColor = texture(texture_diffuse1, fs_in.texCoords);
}