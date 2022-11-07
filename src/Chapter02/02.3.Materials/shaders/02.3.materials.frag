#version 330 core
struct Material {
    // What color the surface reflects under ambient lighting.
    vec3 ambient;
    // The color of the surface under diffuse lighting.
    vec3 diffuse;
    // The color of the specular highlight on the surface
    vec3 specular;
    // Radius of the highlight
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform Material material;
uniform Light light;

uniform vec3 viewPos;

void main() {
    // Ambient
    vec3 ambient = light.ambient * material.ambient;

    // Diffuse
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // Specular
    vec3 viewDirection = normalize(viewPos - FragPos);
    vec3 reflectDir = normalize(reflect(-lightDir, normal));
    float spec = pow(max(dot(viewDirection, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}