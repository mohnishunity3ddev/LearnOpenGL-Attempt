#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 Model, View, Projection;

void main() {
    gl_Position = Projection * View * Model * vec4(aPos, 1.0);
}

