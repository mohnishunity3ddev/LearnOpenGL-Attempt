#version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

const float ambientStrength = 0.1;
const float specularStrength = 0.5f;
const int shininess = 256;

uniform vec3 objectColor;
uniform vec3 lightWorldPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

void main() {
    vec3 ambient = lightColor * ambientStrength;

    // Diffuse
    vec3 lightDir = normalize(lightWorldPos - FragPos);
    float diff = max(dot(Normal, lightDir), 0.0);
    vec3 diffuse = lightColor * diff;
    diffuse /= distance(FragPos, lightWorldPos);

    // Specular
    vec3 viewDirection = normalize(viewPos - FragPos);
    vec3 reflectDir = normalize(reflect(-lightDir, Normal));

    float spec = pow(max(dot(viewDirection, reflectDir), 0.0), shininess);
    vec3 specular = spec * specularStrength * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;

    FragColor = vec4(result, 1.0);
}