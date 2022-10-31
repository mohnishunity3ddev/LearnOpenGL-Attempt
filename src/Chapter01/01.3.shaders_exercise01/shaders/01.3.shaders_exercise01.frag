#version 330 core

out vec4 FragColor;
in vec3 vertexColor;

uniform vec4 uniformColor;

void main() {
    FragColor = vec4(vertexColor * uniformColor.rgb, 1.0f);
}