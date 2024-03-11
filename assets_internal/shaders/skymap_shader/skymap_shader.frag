#version 460 core

out vec4 fragColor;
uniform samplerCube skybox;

in VS_OUT
{
    vec3 texCoords;
} fs_in;

void main()
{
    fragColor = texture(skybox, fs_in.texCoords);
}
