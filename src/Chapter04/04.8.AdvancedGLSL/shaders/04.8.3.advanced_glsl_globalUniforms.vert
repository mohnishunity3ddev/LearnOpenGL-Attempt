#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;
layout(location = 2) in vec3 aNormal;

// Uniform Block of Global Uniforms available to all shaders. std140 defines the
// memory layout. This std140 is the uniform block layout.
layout (std140) uniform Matrices {
    mat4 projection;
    mat4 view;
};

// size and offset of each type of variable in an std140 memory layout.
// these are required to fill data in uniform buffers made on the CPU side.
layout (std140) uniform ExampleBlock
{
                     // base alignment  // aligned offset
    float value;     // 4               // 0 
    vec3 vector;     // 16              // 16  (offset must be multiple of 16 so 4->16)
    mat4 matrix;     // 16              // 32  (column 0)
                     // 16              // 48  (column 1)
                     // 16              // 64  (column 2)
                     // 16              // 80  (column 3)
    float values[3]; // 16              // 96  (values[0])
                     // 16              // 112 (values[1])
                     // 16              // 128 (values[2])
    bool boolean;    // 4               // 144
    int integer;     // 4               // 148
}; 

// interface blocks
out VS_OUT {
    vec2 TexCoords;
} vs_out;

uniform mat4 model;

void main() {
    // Even though these matrices are inside the Matrices struct we can access
    // them directly since these are global uniforms.
    vec4 pos = projection * view * model * vec4(aPos, 1.0);
    gl_Position = pos;
    // If the depth is more, point size 
    gl_PointSize = pos.z;
}