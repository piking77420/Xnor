#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 0) in mat4 instancedArray;



out vec3 WorldPos;

layout (std140, binding = 0) uniform CameraUniform
{
    mat4 view;
    mat4 projection;
    vec3 cameraPos;
    float near;
    float far;
};

layout (std140, binding = 1) uniform ModelUniform
{
    mat4 model;
    mat4 normalInvertMatrix;
    uint drawId;
};



out VS_OUT
{
    vec3 fragPos;
    vec3 normal;
    vec2 texCoords;
} vs_out;


void main()
{
    WorldPos = aPos;
    gl_Position =  projection * view * vec4(WorldPos, 1.0);
}