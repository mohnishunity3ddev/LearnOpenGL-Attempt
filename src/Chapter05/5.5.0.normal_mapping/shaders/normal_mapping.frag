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

uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;

uniform DirectionalLight directional_light;
uniform vec3 viewPos;
uniform float shininess;

vec3 powVec3(vec3 v, int p) {
    return vec3(pow(v.x, p), pow(v.y, p), pow(v.z, p));
}

void main() {
    vec3 color      = texture(diffuseTexture, fs_in.TexCoords).rgb;
    vec3 normalTex  = texture(normalTexture, fs_in.TexCoords).rgb;
    vec3 normal     = normalTex * 2.0 - 1.0;
         normal     = normalize(fs_in.TBN * normal);
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
    float spec      = pow(max(dot(halfwayDir, normal), 0.0), 32);
    vec3 specular   = spec * lightSpecular;

    vec3 lighting   = ambient + diffuse + specular;

    FragColor = vec4(lighting, 1.0);
}