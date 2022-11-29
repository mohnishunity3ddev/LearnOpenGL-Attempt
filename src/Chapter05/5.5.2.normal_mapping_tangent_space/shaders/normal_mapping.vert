#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;


out VS_OUT {
    vec2 TexCoords;
    vec3 FragPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} vs_out;

flat out vec3 TangentLightDirection;

uniform mat4 model;                                                                                                                                
uniform mat4 projection;
uniform mat4 view;

uniform vec3 viewPos;
uniform vec3 lightDirection;

void main() {
    vs_out.TexCoords = aTexCoords;
    vs_out.FragPos   = vec3(model * vec4(aPos, 1.0));

    mat3 normalMatrix = transpose(inverse(mat3(model)));
    
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);

    T = normalize(T - dot(T, N) * N);
    vec3 B  = normalize(normalMatrix * aBitangent);

    if (dot(cross(N, T), B) < 0.0) {
        T = T * -1.0;
    }

    // Matrix to transform positions from tangent-space to world space.
    // mat3 TBN = mat3(T, B, N);

    // Matrix to transform positions from world space to tangent-space.
    // which is basically inverse of the TBN matrix.
    // Inverse of an orthognal matrix(each row is a unit vector and orthogonal to each other) is its transpose.
    mat3 TBN = transpose(mat3(T, B, N));

    TangentLightDirection = TBN * lightDirection;
    vs_out.TangentViewPos = TBN * viewPos;
    vs_out.TangentFragPos = TBN * vs_out.FragPos; 
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}