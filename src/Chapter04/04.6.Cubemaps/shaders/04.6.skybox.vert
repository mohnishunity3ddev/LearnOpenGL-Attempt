#version 330 core
layout(location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main() {
    TexCoords = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    // Here, we make the z component equal to w. So after perspective division,
    // the z(depth value) of the fragment will be 1.
    gl_Position = pos.xyww;
}