#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;


out VS_OUT {
    vec2 TexCoords;
    vec3 FragPos;
    mat3 TBN;
} vs_out;

uniform mat4 model;                                                                                                                                
uniform mat4 projection;
uniform mat4 view;

void main() {
    vs_out.TexCoords = aTexCoords;
    vs_out.FragPos   = vec3(model * vec4(aPos, 1.0));

    mat3 normalMatrix = transpose(inverse(mat3(model)));
    
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);

    T = normalize(T - dot(T, N) * N);
    vec3 B  = normalize(cross(N, T));

    vs_out.TBN = mat3(T, B, N);
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}