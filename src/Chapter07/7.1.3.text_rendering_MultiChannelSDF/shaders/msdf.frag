#version 330 core
out vec4 fragColor;

in vec2 TexCoords;
uniform sampler2D sdfTex;

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

float screenPxRange() {
    float pxRange = 10.0;
    vec2 unitRange = vec2(pxRange) / vec2(textureSize(sdfTex, 0));
    vec2 screenTexSize = vec2(1.0) / fwidth(TexCoords);
    return max(0.5 * dot(unitRange, screenTexSize), 1.0);
}

void main() {
    vec3 fgColor = vec3(1., 1., 1.);
    vec3 msd = texture(sdfTex, TexCoords).rgb;
    float sd = median(msd.r, msd.g, msd.b);
    float screenPxDistance = screenPxRange()*(sd - 0.5);
    float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);
    // fragColor = mix(bgColor, fgColor, opacity);
    fragColor = vec4(fgColor, opacity);
}