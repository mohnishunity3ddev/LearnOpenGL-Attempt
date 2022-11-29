#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec2 TexCoords;
    vec3 FragPos;
    mat3 TBN;
} fs_in;

struct DirectionalLight {
    vec3 direction;
    vec3 color;
    float ambientFactor;
    float diffuseFactor;
    float specularFactor;
};

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_normal1;
};

uniform DirectionalLight directional_light;
uniform vec3 viewPos;
uniform float shininess;
uniform Material material;
uniform mat4 temp;


void main() {
    vec3 color          = texture(material.texture_diffuse1, fs_in.TexCoords).rgb;
    vec3 normalTex      = texture(material.texture_normal1, fs_in.TexCoords).rgb;
    float specularTex   = texture(material.texture_specular1, fs_in.TexCoords).r;

    normalTex = normalize(normalTex);

    vec3 normal = normalize(normalTex * 2.0 - 1.0);
         normal = normalize(fs_in.TBN * normal);
    
    vec3 lightDir   = normalize(directional_light.direction);

    vec3 lightAmbient   = directional_light.color * directional_light.ambientFactor;
    vec3 lightDiffuse   = directional_light.color * directional_light.diffuseFactor;
    vec3 lightSpecular  = directional_light.color * directional_light.specularFactor;

    // ambient
    vec3 ambient    = lightAmbient * color;

    // diffuse
    float diff      = max(dot(-lightDir, normal), 0.0);
    vec3 diffuse    = diff * lightDiffuse * color;

    // specular
    vec3 viewDir    = normalize(viewPos - fs_in.FragPos);
    vec3 halfwayDir = normalize(viewDir + (-lightDir));
    float spec      = pow(max(dot(halfwayDir, normal), 0.0), 512) * specularTex;
    vec3 specular   = spec * lightSpecular;

    vec3 lighting   = ambient + diffuse + specular;

    vec4 t = temp[3];



    mat3 m = mat3(vec3(0, 1, 0), vec3(0, 0, 1), vec3(1, 0, 0));
    vec3 r = m * vec3(0, 1, 0);

    
    FragColor = vec4(lighting, 1.0);
}