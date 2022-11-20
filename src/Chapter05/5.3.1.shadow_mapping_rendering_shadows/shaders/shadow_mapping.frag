#version 330 core
out vec4 fragColor;

in VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoords;
    vec4 fragPosLightSpace;
} fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D depthMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

float shadowCalculation(vec4 fragPosLightSpace) {
    // vertex shader gl_Position does the perspective divide for us. Since
    // lightSpace fragPos was not passed as gl_Position, we have to do the
    // perspective divide ourselves.
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // This will result the projCoords to be in the range [-1, 1]

    // Since we want to use these projCoords to sample the depthMap, we have to
    // convert this into the range [0, 1]
    projCoords = projCoords * 0.5 + 0.5;

    // We use the projCoords we calculated above to sample the depthmap.
    // the returned depth the closest fragment's depth from the point of view of the light.
    float closestDepth = texture(depthMap, projCoords.xy).r;

    // This is the depth of the current fragment we are calculating lighting for
    // from the point of view of the light.
    float currentDepth = projCoords.z;

    // if the current fragment's depth is more than the closest depth the light can see, 
    // that means that this fragment is in its shadow.
    float shadow = currentDepth > closestDepth ? 1.0 : 0.0;

    return shadow;
}   

void main() {
    vec3 color = texture(diffuseTexture, fs_in.texCoords).rgb;
    vec3 normal = normalize(fs_in.normal);
    vec3 lightColor = vec3(0.3);
    // ambient
    vec3 ambient = 0.3 * lightColor;
    // diffuse
    vec3 lightDir = normalize(lightPos - fs_in.fragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // specular
    vec3 viewDir = normalize(viewPos - fs_in.fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;    
    // calculate shadow
    float shadow = shadowCalculation(fs_in.fragPosLightSpace);                      
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;    

    fragColor = vec4(lighting, 1.0);
}