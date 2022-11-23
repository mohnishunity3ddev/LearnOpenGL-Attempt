#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec2 TexCoords;
    vec3 FragPos; 
    vec3 Normal;
} fs_in;

uniform sampler2D diffuseTexture;
uniform samplerCube depthMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float far_plane;

float ShadowCalculation(vec3 fragPos, float bias) {
    vec3 fragToLight = (fragPos - lightPos); // no need to normalize this.
    float closestDepth = texture(depthMap, fragToLight).r;

    // Bringing the depth range from [0, 1] to [0, far_plane].
    closestDepth *= far_plane;

    float currentDepth = length(fragToLight);

    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    return shadow;
}

void main() {
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightColor = vec3(0.3);

    // ambient
    vec3 ambient = 0.3 * color;
    // diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    // specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 halfwayDir = normalize(viewDir + lightDir);
    float spec = pow(max(dot(halfwayDir, normal), 0.0), 64);
    vec3 specular = spec * lightColor;

    float shadowBias = max(0.05 * (1.0 - max(dot(lightDir, normal), 0.0)), 0.005);
    float shadow = ShadowCalculation(fs_in.FragPos, shadowBias);
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;

    FragColor = vec4(lighting, 1.0);
}

