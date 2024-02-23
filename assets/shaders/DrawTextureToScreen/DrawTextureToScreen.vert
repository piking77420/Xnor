#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

const vec2 pos[4]=vec2[4](vec2(-1.0, 1.0),
vec2(-1.0,-1.0),
vec2( 1.0, 1.0),
vec2( 1.0,-1.0));

void main()
{
    TexCoords = 0.5 *pos[gl_VertexID] + vec2(0.5);
    gl_Position= vec4(pos[gl_VertexID], 0.0, 1.0);
}