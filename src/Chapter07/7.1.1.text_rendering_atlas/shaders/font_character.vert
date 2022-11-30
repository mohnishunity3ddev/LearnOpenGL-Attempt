#version 330 core
layout(location = 0) in vec4 aPos;

uniform mat4 model;                                                                                                                                
uniform mat4 projection;
uniform mat4 view;

out vec2 texCoords;

void main() {
    texCoords = aPos.zw;
    vec4 pos = projection * vec4(aPos.xy, 0.0, 1.0);
    gl_Position = pos;
}