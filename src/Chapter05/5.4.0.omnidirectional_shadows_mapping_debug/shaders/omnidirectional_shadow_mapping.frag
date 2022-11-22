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

float shadowCalculation(vec4 fragPosLightSpace, float bias) {
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
    //// float closestDepth = texture(depthMap, projCoords.xy).r;
    // NOTE: This is Percentage Closer Filtering Simple Implementation.
    // Here, we sample around the current fragment from the depth map and
    // average them all to get the final closestDepth.
    float shadow = 0.0;
    
    // Here textureSize returns a vec2 of the width and height of the given
    // sampler texture at mipmap level 0. 1 divided over this returns the size
    // of a single texel that we use to offset the texture coordinates, making
    // sure each new sample samples a different depth value
    vec2 texelSize = 1.0 / textureSize(depthMap, 0);

    // This is the depth of the current fragment we are calculating lighting for
    // from the point of view of the light.
    float currentDepth = projCoords.z;
    
    // NOTE: Solution to OverSampling problem.
    // For regions outside the light's view frustum, the projCoords.z is always greater than 1.
    // Since for the depth map, we already have a border color of 1. projCoords.z or the currentDepth
    // will still be greater than the closestDepth which lies in the region [0, 1].
    // So we clamp the currentDepth to have a max value of 1 to correct this.
    currentDepth = min(currentDepth, 1.0);

    // NOTE: Here, we sample the depth map 9 times around the current fragment
    // with an offset given by the texelSize calculated above and then average
    // the results.
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(depthMap, projCoords.xy + vec2(x, y) * texelSize).r;
            // if the current fragment's depth is more than the closest depth the light can see, 
            // that means that this fragment is in its shadow.
            // shadow bias to remove shadow acne due to the texel resolution of the shadow map.
            shadow += (currentDepth - bias) > pcfDepth ? 1.0 : 0.0;
        }
    }

    shadow /= 9;
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
    // Calculate Shadow:
    // Shadow Acne becomes more prominent when the light is at
    // a steep angle to the surfaces. so we change the value of the shadow bias
    // accordingly. Max bias of 0.05 when light is at angle of 90. Min bias of
    // 0.05 when light has no angle over the surface.
    float shadowBias = max(0.05 * (1 - dot(normal, lightDir)), 0.005);
    float shadow = shadowCalculation(fs_in.fragPosLightSpace, shadowBias);                      
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;    

    fragColor = vec4(lighting, 1.0);
}