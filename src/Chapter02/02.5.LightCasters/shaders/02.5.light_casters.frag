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

uniform Material material;
uniform DirectionalLight directional_light;
uniform PointLight point_light;
uniform AreaLight area_light;

uniform vec3 viewPos;

void main() {
    vec3 diffuseTex  = texture(material.diffuse, TexCoord).rgb;
    vec3 specularTex = texture(material.specular, TexCoord).rgb;
    
    vec3 normal         = normalize(Normal);
    vec3 viewDirection  = normalize(viewPos - FragPos);

    // --------------------------------------------------------------------------------------------------------------------
    // Directional Light
    // --------------------------------------------------------------------------------------------------------------------

    // Ambient
    vec3 ambient    = directional_light.ambient * diffuseTex;
    // Diffuse
    vec3  lightDir  = normalize(-directional_light.direction);
    float diff      = max(dot(normal, lightDir), 0.0);
    vec3  diffuse   = directional_light.diffuse * diff * diffuseTex;
    // Specular
    vec3 reflectDir = normalize(reflect(-lightDir, normal)); // reflect requires the direction of the light which is coming into the normal.
    float spec      = pow(max(dot(viewDirection, reflectDir), 0.0), material.shininess);
    vec3 specular   = directional_light.specular * spec * specularTex;

    vec3 direction_light_col = ambient + diffuse + specular;

    // --------------------------------------------------------------------------------------------------------------------
    // Point Lights
    // --------------------------------------------------------------------------------------------------------------------
    
    vec3 p_ambient  = point_light.ambient * diffuseTex;

    lightDir          = normalize(point_light.position - FragPos);
    diff              = max(dot(normal, lightDir), 0.0);
    vec3 p_diffuse    = point_light.diffuse * diff * diffuseTex;

    reflectDir        = normalize(reflect(-lightDir, normal));
    spec              = pow(max(dot(viewDirection, reflectDir), 0.0), material.shininess);
    vec3 p_specular   = point_light.specular * spec * specularTex;

    float d           = length(point_light.position - FragPos);
    float attenuation = 1.0 / (point_light.constant + (point_light.linear * d) + (point_light.quadratic * d * d));
    
    p_ambient  *= attenuation;
    p_diffuse  *= attenuation;
    p_specular *= attenuation;

    vec3 point_light_col = p_ambient + p_diffuse + p_specular;

    // --------------------------------------------------------------------------------------------------------------------
    // Area Light / Flash Light
    //      LightDir: the vector pointing from the fragment to the light source.
    //      SpotDir: the direction the spotlight is aiming at.
    //      Phi ϕ: the cutoff angle that specifies the spotlight's radius. Everything outside this angle is not lit by the spotlight.
    //      Theta θ: the angle between the LightDir vector and the SpotDir vector. The θ value should be smaller than Φ to be inside the spotligh
    // --------------------------------------------------------------------------------------------------------------------
    vec3  flashlight_col        = vec3(0);
    vec3  flashlight_direction  = normalize(area_light.position - FragPos);
    vec3  flashlight_reverse_forward_direction  = normalize(-area_light.direction);
    float cutoff_threshold      = max(cos(area_light.cutoff), 0.0); // Phi ϕ
    float cutoffAngleDiff       = 4. * (3.14159 / 180.);
    float cutoff_inner          = max(cos(area_light.cutoff - cutoffAngleDiff), 0.0);
    float cutoff_outer          = max(cos(area_light.cutoff + cutoffAngleDiff), 0.0);
    float cutoff_dot            = max(dot(flashlight_reverse_forward_direction, flashlight_direction), 0.0); // Theta θ

    if( cutoff_dot > cutoff_outer ) {
        vec3 f_ambient  = area_light.ambient * diffuseTex;

        diff            = max(dot(normal, flashlight_direction), 0.0);
        vec3 f_diffuse  = area_light.diffuse * diff * diffuseTex;

        reflectDir      = normalize(reflect(-flashlight_direction, normal));
        spec            = pow(max(dot(viewDirection, reflectDir), 0.0), material.shininess);
        vec3 f_specular = area_light.specular * spec * specularTex;

        float f_d       = length(area_light.position - FragPos);
        float f_attenuation = 1.0 / (area_light.constant + (f_d * area_light.linear) + (f_d * f_d * area_light.quadratic));

        f_ambient  *= f_attenuation;
        f_diffuse  *= f_attenuation;
        f_specular *= f_attenuation;

        flashlight_col  = f_ambient + f_diffuse + f_specular;

        vec2 cookieTexCoords = ((FragViewPos.xy / FragViewPos.w) / vec2(2.25, 4)) + vec2(.5, .5);
        // vec2 cookieTexCoords = (((FragViewPos.xy / FragViewPos.w) * vec2(1.25, .7)) + vec2(.5, .5));
        vec3 cookieTex = texture(area_light.cookie, cookieTexCoords).rgb * 4.;
        flashlight_col *= cookieTex;
    }

    vec3 result = flashlight_col + direction_light_col + point_light_col;
    FragColor = vec4(vec3(result), 1.0);
}