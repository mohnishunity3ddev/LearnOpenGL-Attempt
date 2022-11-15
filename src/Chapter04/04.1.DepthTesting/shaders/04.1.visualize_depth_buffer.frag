#version 330 core
out vec4 fragColor;

in vec2 TexCoords;
uniform sampler2D texture1;

float near = 0.1;
float far = 100.0;

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main() {
    float Zeye = LinearizeDepth(gl_FragCoord.z) / far;
    // This is used to see that almost all fragments are white which shows the non-linearity of the depth values.
    fragColor = vec4(vec3(Zeye), 1.0);
}