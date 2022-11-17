#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;
layout(location = 2) in vec3 aNormal;

uniform mat4 model;                                                                                                                                
uniform mat4 projection;
uniform mat4 view;

// interface blocks
out VS_OUT {
    vec2 TexCoords;
} vs_out;

void main() {
    vec4 pos = projection * view * model * vec4(aPos, 1.0);
    gl_Position = pos;
    // If the depth is more, point size 
    gl_PointSize = pos.z;
}