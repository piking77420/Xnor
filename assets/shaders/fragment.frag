#version 460 core

const int MaxSpothLight = 100;
const int MaxPointLight = 100;

struct PointLightData
{
    vec3 color;
    float intensity;
    vec3 position;
    float radius;
};

struct SpothLightData
{
    vec3 color;
    float intensity;
    vec3 position;
    float cutOff;
    vec3 direction;
    float outerCutOff;
};

struct DirectionalData
{
    vec3 color;
    float intensity;
    vec3 position;
    float cutOff;
    vec3 direction;
    float outerCutOff;
};


layout (std140, binding = 2) uniform LightData
{
    int nbrOfPointLight;
    int nbrOfSpothLight;
    PointLightData pointLightData[MaxPointLight];
    SpothLightData spothLightData[MaxSpothLight];
    DirectionalData directionalData;
};


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
    FragColor = vec4(nbrOfPointLight,nbrOfPointLight,nbrOfPointLight,nbrOfPointLight);
}
    



