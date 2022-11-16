#version 330 core
out vec4 fragColor;

in vec3 Normal;
in vec2 TexCoords;
in vec3 FragPosition;

uniform samplerCube skybox;
uniform vec3 eyePosition;

void main() {
    vec3 I = normalize(FragPosition - eyePosition);

    // Reflection Environment Mapping
    vec3 R = reflect(I, normalize(Normal));

    // Refraction Environment Mapping
    // float refractiveIndexRatio = 1.00 / 1.52;
    // vec3 R = refract(I, normalize(Normal), refractiveIndexRatio);
    fragColor = vec4(texture(skybox, R).rgb, 1.);
}