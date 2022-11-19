#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 2) in vec2 aTexCoord;
// This location takes 4 attribute locations. Since max size of an attribute is allowed to be a vec4
// We are storing this mat4 as four separate vec4s. The next attribute (if any) should start with location = 7
// This mat4 takes locations starting with 3 and then 4, 5, 6
layout(location = 3) in mat4 instanceMatrix;

out VS_OUT {
    vec2 texCoords;
} vs_out;

uniform mat4 projection, view;

void main() {
    gl_Position = projection * view * instanceMatrix * vec4(aPos, 1.0);
    vs_out.texCoords = aTexCoord;
}