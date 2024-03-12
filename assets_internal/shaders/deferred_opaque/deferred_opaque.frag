#version 460 core

out vec4 FragColor;

const int MaxSpotLight = 100;
const int MaxPointLight = 100;
const float PI = 3.14159265359;

struct PointLightData
{
    vec3 color;
    float intensity;
    vec3 position;
    float radius;
};

struct SpotLightData
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
    int nbrOfSpotLight;
    int padding1;
    int padding2;
    PointLightData pointLightData[MaxPointLight];
    SpotLightData spotLightData[MaxSpotLight];
    DirectionalData directionalData;
};

vec3 CalcPointLight(PointLightData light, vec3 viewDir, vec3 fragPos, vec3 normal, vec3 albedo)
{
    float distanceLightToFragment = distance(light.position, fragPos);

    if (distanceLightToFragment > light.radius)
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
    vec3 ambient = lightColor * albedo;
    vec3 diffuse = lightColor * diff * albedo;
    vec3 specular = lightColor * spec * albedo;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

vec3 CalcSpotLight(SpotLightData light, vec3 viewDir, vec3 fragPos, vec3 normal, vec3 albedo)
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

    return ambient + diffuse + specular;
}

vec3 CalcDirLight(DirectionalData light, vec3 viewDir, vec3 fragPos, vec3 normal, vec3 albedo)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

    vec3 lightColor = light.color * light.intensity;

    // combine results
    vec3 ambient = lightColor * albedo;
    vec3 diffuse = lightColor * diff * albedo;
    vec3 specular = lightColor  * spec * albedo;

    return ambient + diffuse + specular;
}
// PBR FUMNCTION //
// Specular Component // 
// fr(v,l)=D(h,α)G(v,l,α)F(v,h,f0)4(n⋅v)(n⋅l)

// NDF // D
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}


uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D gMetallicRoughessReflectance;
uniform sampler2D gEmissiveAmbiantOcclusion;


uniform samplerCube SkyBox;

in vec2 texCoords;

void main()
{
    vec3 fragPos = texture(gPosition, texCoords).rgb;
    vec3 normal = texture(gNormal, texCoords).rgb;
    vec3 albedo = texture(gAlbedoSpec, texCoords).rgb;
    
    float metallic = texture(gMetallicRoughessReflectance,texCoords).r;
    float roughness = texture(gMetallicRoughessReflectance,texCoords).g;
    float reflectance = texture(gMetallicRoughessReflectance,texCoords).b;
    
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);
    vec3 Lo = vec3(0.0);
    
    
    // view unit vector
    vec3 v = normalize(cameraPos - fragPos);
    // Incident Light Unit Vector
    vec3 l = normalize(directionalData.direction);
    // Surface Normal
    vec3 n = normalize(normal);
    // half unit vector
    vec3 h = normalize(v + l);
    

    float NoV = abs(dot(n, v)) + 1e-5;
    float NoL = clamp(dot(n, l), 0.0, 1.0);
    float NoH = clamp(dot(n, h), 0.0, 1.0);
    float LoH = clamp(dot(l, h), 0.0, 1.0);
    
    vec3 radiance = vec3(1);
    
    // Cook-Torrance BRDF
    float NDF = DistributionGGX(n, h, roughness);
    float G   = GeometrySmith(n, v, l, roughness);
    vec3 F    = fresnelSchlick(clamp(dot(h, v), 0.0, 1.0), F0);
    vec3 numerator    = NDF * G * F;
    float denominator = 4.0 * max(dot(n, v), 0.0) * max(dot(n, l), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
    vec3 specular = numerator / denominator;


    vec3 i = -v;
    vec3 r = reflect(i, normalize(n));
    vec3 skyBoxReflectedColor = texture(SkyBox, r).rgb;
    
    // kS is equal to Fresnel
    vec3 kS = F * skyBoxReflectedColor;;
    vec3 kD = vec3(1.0) - kS;
    kD *= (1.0 - metallic);

    float NdotL = max(dot(n, l), 0.0);
    Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    

    
    FragColor = vec4(Lo, 1);
}
