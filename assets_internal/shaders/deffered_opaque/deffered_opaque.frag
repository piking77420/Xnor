#version 460 core

out vec4 FragColor;

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
    vec3 direction;
};


layout (std140, binding = 0) uniform CameraUniform
{
    mat4 view;
    mat4 projection;
    vec3 cameraPos;
};


layout (std140, binding = 2) uniform LightData
{
    int nbrOfPointLight;
    int nbrOfSpothLight;
    int padding1;
    int padding2;
    PointLightData pointLightData[MaxPointLight];
    SpothLightData spothLightData[MaxSpothLight];
    DirectionalData directionalData;
};

vec3 CalcPointLight(PointLightData light, vec3 viewDir,vec3 fragPos, vec3 normal,vec3 albedo)
{
    float distanceCamLight = distance(cameraPos, fragPos);

    if(distanceCamLight > light.radius)
        return vec3(0);

    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (distance * distance);

    vec3 lightColor = light.color * light.intensity;

    // combine results
    vec3 ambient  = lightColor * albedo;
    vec3 diffuse  = lightColor * diff * albedo;
    vec3 specular = lightColor  * spec * albedo;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpothLightData light, vec3 viewDir,vec3 fragPos, vec3 normal,vec3 albedo)
{
    
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (distance * distance);
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 lightColor = light.color * light.intensity;

    // combine results
    vec3 ambient = lightColor * albedo;
    vec3 diffuse = lightColor * diff * albedo;
    vec3 specular = lightColor * spec * albedo;
    
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}


vec3 CalcDirLight(DirectionalData light,vec3 viewDir,vec3 fragPos, vec3 normal,vec3 albedo)
{

    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        
    vec3 lightColor = light.color * light.intensity;
    
    // combine results
    vec3 ambient  = lightColor * albedo;
    vec3 diffuse  = lightColor * diff * albedo;
    vec3 specular = lightColor  * spec * albedo;
    return (ambient + diffuse + specular);
}

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

in vec2 TexCoords;

void main()
{


    vec3 FragPos = texture(gPosition,TexCoords).rgb;
    vec3 Normal = texture(gNormal,TexCoords).rgb;
    vec3 Albedo = texture(gAlbedoSpec,TexCoords).rgb;
    vec3 viewDir = normalize(cameraPos - FragPos);


    vec3 finalColor = CalcDirLight(directionalData,viewDir,FragPos,Normal,Albedo);
    
    for (int i = 0 ; i < nbrOfPointLight ; i++)
    {
        finalColor += CalcPointLight(pointLightData[i],viewDir,FragPos,Normal,Albedo);
    }

    for (int i = 0 ; i < nbrOfSpothLight ; i++)
    {
        finalColor += CalcSpotLight(spothLightData[i],viewDir,FragPos,Normal,Albedo);
    }
    
    
    FragColor = vec4(finalColor,1);
}
    
