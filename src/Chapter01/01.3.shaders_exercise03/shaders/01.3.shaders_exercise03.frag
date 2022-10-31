#version 330 core

out vec4 FragColor;
in float offset;

void main() {
    FragColor = vec4(vec3(abs(offset * 2.)), 1.0f);
}