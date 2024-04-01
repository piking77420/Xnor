#version 460 core
layout (location = 0) out float PixelLigthDistance;


in vec3 WorldPos;
uniform vec3 CameraPos;

void main()
{
    //gl_FragDepth = gl_FragCoord.z;
    vec3 pixelToVertex = WorldPos - CameraPos;
    PixelLigthDistance = length(pixelToVertex);
}
