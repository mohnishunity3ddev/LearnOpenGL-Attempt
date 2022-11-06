#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Lighting;

uniform mat4 model, view, projection;
uniform vec3 lightWorldPos, lightColor;
uniform vec3 objectColor;
uniform vec3 camWorldPos;

const float ambientStrength = 0.1;
const float specularStrength = .5;
const int shininess = 512;

void main() {
   gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);

   vec3 FragPos = vec3(model * vec4(aPos, 1.0)); 
   vec3 Normal = mat3(transpose(inverse(model))) * aNormal;

   vec3 ambient = lightColor * ambientStrength;

    // Diffuse
    vec3 lightDir = normalize(lightWorldPos - FragPos);
    float diff = max(dot(Normal, lightDir), 0.0);
    vec3 diffuse = lightColor * diff;
    diffuse /= distance(FragPos, lightWorldPos);

    // Specular
    vec3 viewDirection = normalize(camWorldPos - FragPos);
    vec3 reflectDir = normalize(reflect(-lightDir, Normal));
    float spec = pow(max(dot(viewDirection, reflectDir), 0.0), shininess);
    vec3 specular = spec * specularStrength * lightColor;

    Lighting = ambient + diffuse + specular;
}