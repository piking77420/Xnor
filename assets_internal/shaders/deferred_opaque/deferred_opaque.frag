#version 460 core

out vec4 FragColor;

const int MaxSpotLight = 100;
const int MaxPointLight = 100;
const float PI = 3.14159265359;
const float InvPI = 1/PI;

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


// PBR FUNCTION //
// Specular Component // 
// fr(v,l)=D(h,α)G(v,l,α)F(v,h,f0)4(n⋅v)(n⋅l)
//Diffuse BRDF
//fd(l, v) = cdiff/π

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D gMetallicRoughessReflectance;
uniform sampler2D gEmissiveAmbiantOcclusion;


uniform samplerCube irradianceMap;

in vec2 texCoords;

//https://de45xmedrsdbp.cloudfront.net/Resources/files/2013SiggraphPresentationsNotes-26915738.pdf
// reparameterization of α = Roughness^2
float SpecularD(float NoH,float a)
{
    float numerator = a * a;
    float denominator = PI * pow( pow(NoH,2) * (numerator - 1) + 1,2);
        
    return numerator / denominator;
}

float SpecularG1(vec3 n,vec3 v,float k)
{
    float NoV = clamp(dot(n,v), 0.0, 1.0);
    
    return NoV / ( NoV * ( 1 - k) + k );
}

float SpecularG(vec3 l, vec3 v, vec3 h, vec3 n, float roughness)
{
    float k = pow((roughness + 1),2) / 8f;
    
    return SpecularG1(n,l,k) * SpecularG1(n,v,k);
}
vec3 SpecularF(float VoH,vec3 F0)
{
    float power = (-5.5473 * VoH - 6.98316) * VoH;
    return F0 + (1 - F0) * pow(2,power); 
}


void main()
{
    vec3 fragPos = texture(gPosition, texCoords).rgb;
    vec3 normal = texture(gNormal, texCoords).rgb;
    vec3 albedo = texture(gAlbedoSpec, texCoords).rgb;
    
    float metallic = texture(gMetallicRoughessReflectance,texCoords).r;
    float roughness = texture(gMetallicRoughessReflectance,texCoords).g;
    float reflectance = texture(gMetallicRoughessReflectance,texCoords).b;

    float emissive = texture(gEmissiveAmbiantOcclusion,texCoords).r;
    float ambientOcclusion = texture(gEmissiveAmbiantOcclusion,texCoords).g;


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
    
    float NoH = clamp(dot(n,h),0.0,1.0);
    float VoH = clamp(dot(v,h),0.0,1.0);

    float ndf = SpecularD(NoH, roughness * roughness);
    float g =  SpecularG(l, v, h, n, roughness);
    vec3 f = SpecularF(VoH,F0);

    vec3 numerator    = ndf * g * f;
    float denominator = 4.0 * max(dot(n, v), 0.0) * max(dot(n, l), 0.0) + 0.0001;
    vec3 specular  = numerator / denominator;


    vec3 kS = f;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;
    float NdotL = max(dot(n, l), 0.0);
    
    Lo += (kD * albedo * InvPI + specular) * NdotL;
    
    
    FragColor = vec4(Lo, 1);
}
