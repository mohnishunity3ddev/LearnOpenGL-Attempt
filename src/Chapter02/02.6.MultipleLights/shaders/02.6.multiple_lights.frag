#version 330 core
struct Material {
    // The color of the surface under diffuse lighting.
    sampler2D diffuse;
    // The color of the specular highlight on the surface
    sampler2D specular;
    sampler2D emission;
    // Radius of the highlight
    float shininess;
};

struct DirectionalLight {
    // Position is not needed for directional lights which are far away. And all light rays are more or less parallel.
    // vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    // for attenuation purposes.
    float constant;
    float linear;
    float quadratic;
};

struct AreaLight {
    vec3 position;
    vec3 direction;
    float cutoff; // cutoff angle of the area light/ defines the volume of the flashlight cone.
    sampler2D cookie;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
in vec4 FragViewPos;

out vec4 FragColor;

#define NR_POINT_LIGHTS 4

uniform Material material;
uniform DirectionalLight directional_light;
uniform PointLight point_lights[NR_POINT_LIGHTS];
uniform AreaLight area_light;

uniform vec3 viewPos;

vec3 calculateDirectionalLight(DirectionalLight light, vec3 diffuseTextureColor, vec3 specularTextureColor, vec3 normal, vec3 viewDirection);
vec3 calculatePointLight(PointLight light, vec3 diffuseTextureColor, vec3 specularTextureColor, vec3 normal, vec3 viewDirection);
vec3 calculateSpotLight(AreaLight light, vec3 diffuseTextureColor, vec3 specularTextureColor, vec3 normal, vec3 viewDirection);

void main() {
    vec3 diffuseTex  = texture(material.diffuse, TexCoord).rgb;
    vec3 specularTex = texture(material.specular, TexCoord).rgb;
    
    vec3 normal         = normalize(Normal);
    vec3 viewDirection  = normalize(viewPos - FragPos);

    vec3 result = vec3(0.);

    result += calculateDirectionalLight(directional_light, diffuseTex, specularTex, normal, viewDirection);
    for(int i = 0; i < NR_POINT_LIGHTS; ++i) {
        result += calculatePointLight(point_lights[i], diffuseTex, specularTex, normal, viewDirection);
    }
    result += calculateSpotLight(area_light, diffuseTex, specularTex, normal, viewDirection);

    FragColor = vec4(result, 1.0);
}

// --------------------------------------------------------------------------------------------------------------------
// Directional Light
// --------------------------------------------------------------------------------------------------------------------
vec3 calculateDirectionalLight(DirectionalLight dirLight, vec3 diffuseTextureColor, vec3 specularTextureColor, vec3 normal, vec3 viewDirection) {
    // Ambient
    vec3 ambient    = dirLight.ambient * diffuseTextureColor;
    // Diffuse
    vec3  lightDir  = normalize(-dirLight.direction);
    float diff      = max(dot(normal, lightDir), 0.0);
    vec3  diffuse   = dirLight.diffuse * diff * diffuseTextureColor;
    // Specular
    vec3 reflectDir = normalize(reflect(-lightDir, normal)); // reflect requires the direction of the light which is coming into the normal.
    float spec      = pow(max(dot(viewDirection, reflectDir), 0.0), material.shininess);
    vec3 specular   = dirLight.specular * spec * specularTextureColor;

    vec3 direction_light_col = ambient + diffuse + specular;
    return direction_light_col;
}

// --------------------------------------------------------------------------------------------------------------------
// Point Lights
// --------------------------------------------------------------------------------------------------------------------
vec3 calculatePointLight(PointLight light, vec3 diffuseTextureColor, vec3 specularTextureColor, vec3 normal, vec3 viewDirection) {
    vec3 p_ambient  = light.ambient * diffuseTextureColor;

    vec3 lightDir     = normalize(light.position - FragPos);
    float diff        = max(dot(normal, lightDir), 0.0);
    vec3 p_diffuse    = light.diffuse * diff * diffuseTextureColor;

    vec3 reflectDir   = normalize(reflect(-lightDir, normal));
    float spec        = pow(max(dot(viewDirection, reflectDir), 0.0), material.shininess);
    vec3 p_specular   = light.specular * spec * specularTextureColor;

    float d           = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + (light.linear * d) + (light.quadratic * d * d));
    
    p_ambient  *= attenuation;
    p_diffuse  *= attenuation;
    p_specular *= attenuation;

    vec3 point_light_col = p_ambient + p_diffuse + p_specular;
    return point_light_col;
}

// --------------------------------------------------------------------------------------------------------------------
// Area Light / Flash Light
//      LightDir: the vector pointing from the fragment to the light source.
//      SpotDir: the direction the spotlight is aiming at.
//      Phi ϕ: the cutoff angle that specifies the spotlight's radius. Everything outside this angle is not lit by the spotlight.
//      Theta θ: the angle between the LightDir vector and the SpotDir vector. The θ value should be smaller than Φ to be inside the spotligh
// --------------------------------------------------------------------------------------------------------------------
vec3 calculateSpotLight(AreaLight light, vec3 diffuseTextureColor, vec3 specularTextureColor, vec3 normal, vec3 viewDirection) {
    vec3  flashlight_col        = vec3(0);
    vec3  flashlight_direction  = normalize(light.position - FragPos);
    vec3  flashlight_reverse_forward_direction  = normalize(-light.direction);
    float cutoff_threshold      = max(cos(area_light.cutoff), 0.0); // Phi ϕ
    float cutoffAngleDiff       = 4. * (3.14159 / 180.);
    float cutoff_inner          = max(cos(light.cutoff - cutoffAngleDiff), 0.0);
    float cutoff_outer          = max(cos(light.cutoff + cutoffAngleDiff), 0.0);
    float cutoff_dot            = max(dot(flashlight_reverse_forward_direction, flashlight_direction), 0.0); // Theta θ

    if( cutoff_dot > cutoff_outer ) {
        vec3 f_ambient  = light.ambient * diffuseTextureColor;

        float diff      = max(dot(normal, flashlight_direction), 0.0);
        vec3 f_diffuse  = light.diffuse * diff * diffuseTextureColor;

        vec3 reflectDir = normalize(reflect(-flashlight_direction, normal));
        float spec      = pow(max(dot(viewDirection, reflectDir), 0.0), material.shininess);
        vec3 f_specular = light.specular * spec * specularTextureColor;

        float f_d       = length(light.position - FragPos);
        float f_attenuation = 1.0 / (light.constant + (f_d * light.linear) + (f_d * f_d * light.quadratic));

        f_ambient  *= f_attenuation;
        f_diffuse  *= f_attenuation;
        f_specular *= f_attenuation;

        vec3 flashlight_col  = f_ambient + f_diffuse + f_specular;

        vec2 cookieTexCoords = ((FragViewPos.xy / FragViewPos.w) / vec2(2.25, 4)) + vec2(.5, .5);
        // vec2 cookieTexCoords = (((FragViewPos.xy / FragViewPos.w) * vec2(1.25, .7)) + vec2(.5, .5));
        vec3 cookieTex = texture(light.cookie, cookieTexCoords).rgb * 4.;
        flashlight_col *= cookieTex;

        return flashlight_col;
    }
}