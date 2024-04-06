#version 460 core
layout (location = 0) in vec3 aPos;

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


void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
