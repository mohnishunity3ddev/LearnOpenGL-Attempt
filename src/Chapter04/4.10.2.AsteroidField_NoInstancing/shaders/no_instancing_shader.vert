#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out VS_OUT {
    vec2 texCoords;
} vs_out;

uniform mat4 projection, view, model;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vs_out.texCoords = aTexCoord;
}