#version 330 core
out vec4 fragColor;

in VS_OUT {
    vec2 texCoords;
    vec3 normal;
    vec3 fragPos;
} fs_in;

struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
    sampler2D diffuse;
    float shininess;
};

uniform DirectionalLight directional_light;
uniform Material material;
uniform vec3 cameraPos;
uniform bool useBlinnPhong;

void main() {
    vec3 diffuseTex = texture(material.diffuse, fs_in.texCoords).rgb;

    vec3 reverseLightDir = -normalize(directional_light.direction);
    vec3 normal = normalize(fs_in.normal);
    vec3 fragmentWorldPos = fs_in.fragPos;

    vec3 ambient = directional_light.ambient * diffuseTex;

    float diff = max(dot(normal, reverseLightDir), 0.0);
    vec3 diffuse = diff * directional_light.diffuse * diffuseTex;

    float spec = 0.0;
    vec3 viewDirection = normalize(cameraPos - fragmentWorldPos);

    if (useBlinnPhong) {
        vec3 halfwayDirection   = normalize(viewDirection + reverseLightDir);
        spec = pow(max(dot(halfwayDirection, normal), 0.0), material.shininess);
    } else {
        vec3 reflectDirection = normalize(reflect(-reverseLightDir, normal));
        
        spec = pow(max(dot(reflectDirection, viewDirection), 0.0), material.shininess);
    }

    vec3 specular = spec * directional_light.specular; 

    vec3 result = ambient + diffuse + specular;
    fragColor = vec4(result, 1.0);
}