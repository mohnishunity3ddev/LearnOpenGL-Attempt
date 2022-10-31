#version 330 core

layout (location = 0) in vec3 aPos;

uniform float xOffset;
out float offset;

void main() {
   offset = xOffset;
   gl_Position = vec4(aPos.x + offset, aPos.y, aPos.z, 1.0);
}