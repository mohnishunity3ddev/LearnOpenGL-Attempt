#version 330 core
in vec3 Lighting;

uniform vec3 objectColor;

out vec4 FragColor;

void main() {
    FragColor = vec4(objectColor * Lighting, 1.0);
}