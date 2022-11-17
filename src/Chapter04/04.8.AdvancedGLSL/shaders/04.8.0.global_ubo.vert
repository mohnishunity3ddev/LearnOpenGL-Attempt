#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;
layout(location = 2) in vec3 aNormal;

// Uniform Buffer Objects
layout (std140) uniform Matrices {
    uniform mat4 projection;
    uniform mat4 view;
};

uniform mat4 model;                                                                                                                                
void main() {
    vec4 pos = projection * view * model * vec4(aPos, 1.0);
    gl_Position = pos;
}