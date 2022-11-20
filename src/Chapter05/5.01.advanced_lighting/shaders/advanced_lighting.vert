#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;
layout(location = 2) in vec3 aNormal;

out VS_OUT {
    vec2 texCoords;
    vec3 normal;
    vec3 fragPos;
} vs_out;

uniform mat4 model;                                                                                                                                
uniform mat4 projection;
uniform mat4 view;

void main() {
    vs_out.texCoords    = aTexCoords;
    vs_out.normal       = mat3(transpose(inverse(model))) * aNormal;
    vs_out.fragPos      = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}