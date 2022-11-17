#version 330 core
out vec4 fragColor;

in VS_OUT {
    vec2 TexCoords;
} fs_in;

void main() {
    fragColor = vec4(1., 0, 1., 1.);
}