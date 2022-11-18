#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out VS_OUT {
    vec3 normal;
} vs_out;

uniform mat4 view, model;

void main() {
    gl_Position = view * model * vec4(aPos, 1.0);
    vs_out.normal = mat3(transpose(inverse(view * model))) * aNormal;
}