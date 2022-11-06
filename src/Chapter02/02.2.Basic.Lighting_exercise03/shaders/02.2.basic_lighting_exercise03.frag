#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec3 LightViewSpacePos;

out vec4 FragColor;

const float ambientStrength = 0.1;
const float specularStrength = .5;
const int shininess = 512;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main() {
    vec3 ambient = lightColor * ambientStrength;

    // Diffuse
    vec3 lightDir = normalize(LightViewSpacePos - FragPos);
    float diff = max(dot(Normal, lightDir), 0.0);
    vec3 diffuse = lightColor * diff;
    diffuse /= distance(FragPos, LightViewSpacePos);

    // Specular
    vec3 viewDirection = normalize(-FragPos);
    vec3 reflectDir = normalize(reflect(-lightDir, Normal));

    float spec = pow(max(dot(viewDirection, reflectDir), 0.0), shininess);
    vec3 specular = spec * specularStrength * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;

    FragColor = vec4(result, 1.0);
}