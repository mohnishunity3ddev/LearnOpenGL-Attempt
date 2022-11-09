#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 Normal;
out vec3 FragPos;
out vec4 FragViewPos;
out vec2 TexCoord;

uniform mat4 model, view, projection;
uniform mat4 flashlightProj;

void main() {
   FragPos = vec3(model * vec4(aPos, 1.0)); 
   FragViewPos = flashlightProj * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
   Normal = mat3(transpose(inverse(model))) * aNormal;
   TexCoord = aTexCoord;
   
   vec4 outPos = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
   gl_Position = outPos;
}