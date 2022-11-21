#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;
layout(location = 2) in vec3 aNormal;

out vec2 TexCoords;

uniform mat4 model;                                                                                                                                
uniform mat4 projection;
uniform mat4 view;

void main() {
    TexCoords = aTexCoords;
    vec4 pos = vec4(aPos, 1.0);
    gl_Position = pos;
}