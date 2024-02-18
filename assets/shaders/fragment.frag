#version 460 core

/*
readonly restrict layout(std430) buffer u_textureSamplerers
{
    uvec2 textureSamplerers[];
};*/


in VS_OUT 
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

out vec4 FragColor;

uniform vec3 color;
uniform sampler2D diffuseTexture;

void main()
{
    vec4 ambiant = texture(diffuseTexture,fs_in.TexCoords);
    FragColor = vec4(ambiant);
}
    



