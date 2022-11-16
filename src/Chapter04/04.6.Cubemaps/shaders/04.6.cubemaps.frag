#version 330 core
out vec4 fragColor;

in vec3 Normal;
in vec2 TexCoords;
in vec3 FragPosition;

uniform samplerCube skybox;
uniform vec3 eyePosition;

void main() {
    vec3 I = normalize(FragPosition - eyePosition);
    vec3 R = reflect(I, normalize(Normal));
    fragColor = vec4(texture(skybox, R).rgb, 1.);
}