#version 330 core
out vec4 fragColor;

in vec2 texCoords;

uniform sampler2D fontAtlas;

void main() {
    float fontCol = texture(fontAtlas, texCoords).r;
    vec4 sample = vec4(1, 1, 1, fontCol); 
    // fragColor = vec4(vec3(fontCol), 1.);
    fragColor = vec4(vec3(1.), 1.) * sample;// + vec4(TexCoords, 0., 0.);
}