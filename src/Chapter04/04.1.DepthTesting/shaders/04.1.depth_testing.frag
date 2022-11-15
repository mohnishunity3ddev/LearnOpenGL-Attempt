#version 330 core
out vec4 fragColor;

in vec2 TexCoords;
uniform sampler2D texture1;

void main() {
    vec3 tex = texture(texture1, TexCoords).rgb;
    fragColor = vec4(tex, 1.0);
}