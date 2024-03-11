#version 460 core
#extension GL_NV_gpu_shader5 : require

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 texCoords;

const vec2 pos[] =
{
    vec2(-1.0,  1.0),
    vec2(-1.0, -1.0),
    vec2( 1.0,  1.0),
    vec2( 1.0, -1.0)
};

void main()
{
    texCoords = 0.5 * pos[gl_VertexID] + vec2(0.5);
    gl_Position = vec4(pos[gl_VertexID], 0.0, 1.0);
}
