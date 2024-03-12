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
float D_GGX(float NoH, float roughness) {
    float a = NoH * roughness;
    float k = roughness / (1.0 - NoH * NoH + a * a);
    return k * k * (1.0 / PI);
}

//  Geometric shadowing // G
float V_SmithGGXCorrelated(float NoV, float NoL, float roughness) {
    float a2 = roughness * roughness;
    float GGXV = NoL * sqrt(NoV * NoV * (1.0 - a2) + a2);
    float GGXL = NoV * sqrt(NoL * NoL * (1.0 - a2) + a2);
    return 0.5 / (GGXV + GGXL);
}
// Fresnel term // F
// defines how light reflects and refracts at the interface between 
vec3 F_Schlick(float u, vec3 f0) {
    float f = pow(1.0 - u, 5.0);
    return f + f0 * (1.0 - f);
}
// Diffuse Component // 
// diffuse Lambertian // 
float Fd_Lambert() {
    return 1.0 / PI;
}
// diffuse Lambertian Disney // 
float F_Schlick(float u, float f0, float f90) {
    return f0 + (f90 - f0) * pow(1.0 - u, 5.0);
}

float Fd_Burley(float NoV, float NoL, float LoH, float roughness)
{
    float f90 = 0.5 + 2.0 * roughness * LoH * LoH;
    float lightScatter = F_Schlick(NoL, 1.0, f90);
    float viewScatter = F_Schlick(NoV, 1.0, f90);
    return lightScatter * viewScatter * (1.0 / PI);
}



uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D gMetallicRoughessReflectance;
uniform sampler2D gEmissiveAmbiantOcclusion;

in vec2 texCoords;

void main()
{
    vec3 fragPos = texture(gPosition, texCoords).rgb;
    vec3 normal = texture(gNormal, texCoords).rgb;
    vec3 albedo = texture(gAlbedoSpec, texCoords).rgb;
    
    float metallic = texture(gMetallicRoughessReflectance,texCoords).r;
    float perceptualRoughness = texture(gMetallicRoughessReflectance,texCoords).g;
    float reflectance = texture(gMetallicRoughessReflectance,texCoords).b;
    
    float roughness = perceptualRoughness * perceptualRoughness;
    
    vec3 f0 = 0.16 * reflectance * reflectance * (1.0 - metallic) + (albedo * metallic);
    
    
    // view unit vector
    vec3 v = normalize(cameraPos - fragPos);
    // Incident Light Unit Vector
    vec3 l = normalize(directionalData.direction);
    // Surface Normal
    vec3 n = normalize(normal);
    // half unit vector
    vec3 h = normalize(v + l);


    float NoV = abs(dot(n, v));
    float NoL = clamp(dot(n, l), 0.0, 1.0);
    float NoH = clamp(dot(n, h), 0.0, 1.0);
    float LoH = clamp(dot(l, h), 0.0, 1.0);


    float D = D_GGX(NoH, roughness);
    vec3  F = F_Schlick(LoH, f0);
    float V = V_SmithGGXCorrelated(NoV, NoL, roughness);

    // specular BRDF
    vec3 Fr = (D * V) * F;
    
    /*
    // Single MultiScatering 
    vec3 energyCompensation = 1.0 + f0 * (1.0 / dfg.y - 1.0);
    // Scale the specular lobe to account for multiscattering
    Fr *= pixel.energyCompensation;
*/

    vec3 diffuseColor = (1.0 - metallic) * albedo.rgb;
    // diffuse BRDF
    vec3 Fd = diffuseColor * Fd_Burley(NoV, NoL, LoH, roughness);

    FragColor = vec4(Fr + Fd, 1);
}
