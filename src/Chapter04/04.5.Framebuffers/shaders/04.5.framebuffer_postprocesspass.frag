#version 330 core
out vec4 fragColor;

in vec2 TexCoords;
uniform sampler2D screenTexture;

const float offset = 1.0 / 300.0;

const vec2 offsets[9] = vec2[9] (
    vec2(-offset, -offset),
    vec2( 0.0,    -offset),
    vec2( offset, -offset),
    vec2(-offset,  0.0),
    vec2( 0.0,     0.0),
    vec2( offset,  0.0),
    vec2(-offset,  offset),
    vec2( 0.0,     offset),
    vec2( offset,  offset)
);

// Color Inversion
vec4 postProcess_Invert(const vec4 inColor) {
    vec4 outColor = vec4(vec3(1. - inColor), 1.);
    return outColor;
}

// Grayscale post process effect using weighted colors.
float postProcess_Grayscale(const vec4 inColor) {
    float outColor = 0.2126 * inColor.r + 0.7152 * inColor.g + 0.0722 * inColor.b;
    return outColor;
}

// Kernel-Effects
const float sharpenKernel[9] = float[] (
    -1, -1, -1,
    -1,  9, -1,
    -1, -1, -1
);

vec3 postProcess_Sharpen() {
    vec3 sampleTex[9];
    for(int i = 0; i < 9; ++i) {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }

    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; ++i) {
        col += sampleTex[i] * sharpenKernel[i];
    }
    return col;
}

const float blurKernel[9] = float[9] (
    1.0 / 16.0,   2.0 / 16.0,    1.0 / 16.0,
    2.0 / 16.0,   4.0 / 16.0,    2.0 / 16.0,
    1.0 / 16.0,   2.0 / 16.0,    1.0 / 16.0
);

vec3 postProcess_Blur() {
    vec3 sampleTex[9];
    for(int i = 0; i < 9; ++i) {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }

    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; ++i) {
        col += sampleTex[i] * blurKernel[i];
    }
    return col;
}

const float edgeDetectKernel[9] = float[9] (
    1,   1,  1,
    1,  -8,  1,    
    1,   1,  1
);
vec3 postProcess_EdgeDetection() {
    vec3 sampleTex[9];
    for(int i = 0; i < 9; ++i) {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }

    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; ++i) {
        col += sampleTex[i] * edgeDetectKernel[i];
    }
    return col;
}




void main() {
    vec4 texColor = texture(screenTexture, TexCoords);

    // Color inversion effect.
    // fragColor = postProcess_Invert(texColor);

    // Grayscale effect.
    // float grayscale = postProcess_Grayscale(texColor);
    // fragColor = vec4(vec3(grayscale), 1.);

    // Sharpen Kernel Effect.
    vec3 col = postProcess_EdgeDetection();
    fragColor = vec4(col, 1.);
}