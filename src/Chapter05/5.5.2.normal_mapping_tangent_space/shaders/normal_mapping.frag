#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec2 TexCoords;
    vec3 FragPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;
flat in vec3 TangentLightDirection;

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
uniform float shininess;

vec3 powVec3(vec3 v, int p) {
    return vec3(pow(v.x, p), pow(v.y, p), pow(v.z, p));
}

void main() {
    vec3 color      = texture(diffuseTexture, fs_in.TexCoords).rgb;
    vec3 normalTex  = texture(normalTexture, fs_in.TexCoords).rgb;
    // This normal will be in tangent space. Since all calculations are in
    // tangent space - this is all we need for the normal.
    vec3 normal     = normalize(normalTex * 2.0 - 1.0);
    
    vec3 lightDir   = normalize(TangentLightDirection);

    vec3 lightAmbient   = directional_light.color * directional_light.ambientFactor;
    vec3 lightDiffuse   = directional_light.color * directional_light.diffuseFactor;
    vec3 lightSpecular  = directional_light.color * directional_light.specularFactor;

    // ambient
    vec3 ambient    = lightAmbient * color;

    // diffuse
    float diff      = max(dot((-lightDir), normal), 0.0);
    vec3 diffuse    = diff * lightDiffuse * color;

    // specular
    vec3 viewDir    = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3 halfwayDir = normalize(viewDir + (-lightDir));
    float spec      = pow(max(dot(halfwayDir, normal), 0.0), shininess);
    vec3 specular   = spec * lightSpecular;

    vec3 lighting   = ambient + diffuse + specular;

    // float viewDot = pow(max(dot(-lightDir, normal), 0.0), 8);
    // FragColor = vec4(vec3(pow(normalTex.z, 512)), 1.0);
    FragColor = vec4(lighting, 1.0);
}