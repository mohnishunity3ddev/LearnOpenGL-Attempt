#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;
layout(location = 2) in vec3 aNormal;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPosition;

uniform mat4 model, view, projection;

void main() {
    Normal = mat3(transpose(inverse(model))) * aNormal;
    FragPosition = vec3(model * vec4(aPos, 1.0));
    TexCoords = aTexCoords;
    gl_Position = projection * view * model * vec4(aPos, 1.);
}