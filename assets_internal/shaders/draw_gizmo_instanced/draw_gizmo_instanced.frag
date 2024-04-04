#version 330 core

out vec4 FragColor;


in VS_OUT
{
    vec3 fragPos;
    vec3 normal;
    vec2 texCoords;
} fs_in;

void main()
{
    

    FragColor = vec4(prefilteredColor, 1.0);
}