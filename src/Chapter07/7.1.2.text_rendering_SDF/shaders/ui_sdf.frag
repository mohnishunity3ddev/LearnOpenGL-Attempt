#version 330 core
out vec4 fragColor;

in vec2 TexCoords;

uniform sampler2D fontTexture;
uniform vec3 outlineColor;
uniform vec3 fontColor;

void main() {
    float sdfSample = texture(fontTexture, TexCoords).r;
    if(sdfSample >= 0.5) {
        fragColor = vec4(fontColor, 1.);
    } else if(sdfSample > 0.495 && sdfSample < 0.5) {
        float smoothC1 = smoothstep(0.495, 0.5, sdfSample);
        fragColor = vec4(fontColor, smoothC1);
    } else if(sdfSample > 0.47 && sdfSample <= 0.495) {
        float smoothC2 = smoothstep(0.47, 0.495, sdfSample);
        fragColor = vec4(outlineColor, smoothC2);
    } else if(sdfSample <= 0.47) {
        fragColor = vec4(0);
    }
}