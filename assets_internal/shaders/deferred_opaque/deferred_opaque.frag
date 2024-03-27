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
    bool isCastShadow;
    mat4 lightSpaceMatrix;
};

struct DirectionalData
{
    vec3 color;
    float intensity;
    vec3 direction;
    bool isDirlightCastShadow;
    mat4 lightSpaceMatrix;
};

layout (std140, binding = 2) uniform LightData
{
    int nbrOfPointLight;
    int nbrOfSpotLight;
    PointLightData pointLightData[MaxPointLight];
    SpotLightData spotLightData[MaxSpotLight];
    DirectionalData directionalData;
};
layout (std140, binding = 0) uniform CameraUniform
{
    mat4 view;
    mat4 projection;
    vec3 cameraPos;
};


uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D gMetallicRoughessReflectance;
uniform sampler2D gAmbiantOcclusion;
uniform sampler2D gEmissive;


uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLUT;

uniform sampler2D dirLightShadowMap;
uniform sampler2DArray SpotLightShadowArray;
in vec2 texCoords;

float ShadowCalculation(vec4 fragPosLightSpace, vec3 n, vec3 l)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(dirLightShadowMap, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(n);
    vec3 lightDir = normalize(l);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // check whether current frag pos is in shadow
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    vec2 texelSize = 1.0 / textureSize(dirLightShadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(dirLightShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
    shadow = 0.0;

    return shadow;
}

float ShadowCalculationSpolight(vec4 fragPosLightSpace, vec3 n, vec3 l,int index)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(SpotLightShadowArray, vec3(projCoords.x, projCoords.y, index)).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(n);
    vec3 lightDir = normalize(l);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // check whether current frag pos is in shadow
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    vec2 texelSize = 1.0 / textureSize(SpotLightShadowArray, 0).xy;
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            vec2 texel = projCoords.xy + vec2(x, y) * texelSize;
            float pcfDepth = texture(SpotLightShadowArray, vec3(texel,index)).r;
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
    shadow = 0.0;

    return shadow;
}




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
    float k = pow((roughness + 1.0), 2.0) / 8.0;

    return SpecularG1(n,l,k) * SpecularG1(n,v,k);
}
vec3 SpecularF(float VoH,vec3 F0, float roughness)
{
    float power = (-5.5473 * VoH - 6.98316) * VoH;
    return F0 + ((1 - F0) - roughness) * pow(2,power); 
}

vec3 ComputeIbl(float roughness,vec3 kD, float ao,vec3 albedo, vec3 N , vec3 R ,vec3 V,vec3 F) 
{
    vec3 irradiance = texture(irradianceMap, N).rgb;
    vec3 diffuse    = irradiance * albedo;
    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(prefilterMap, R,  roughness * MAX_REFLECTION_LOD).rgb;
    vec2 brdf  = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
    vec3 specularAmbiant = prefilteredColor * (F * brdf.x + brdf.y);

    vec3 ambient = (kD * diffuse + specularAmbiant) * ao;

    return ambient;
}

vec3 ComputeSpotLight(vec3 baseColor,vec4 fragPos,vec3 v, vec3 n, float roughness, float metallic, vec3 f0)
{
    vec3 outLo = vec3(0.f);
    vec3 fragposVec3 = vec3(fragPos.x, fragPos.y, fragPos.z);

    for (int i = 0; i < nbrOfSpotLight; i++)
    {
        SpotLightData light = spotLightData[i];
        float distance = length(light.position - fragposVec3);
        
        vec3 l = normalize(vec3(light.position - fragposVec3));
        vec3 h = normalize(v + l);

        float theta = dot(l, normalize(-light.direction));
        float epsilon = light.cutOff - light.outerCutOff;
        float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);


        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = light.color * attenuation * light.intensity * intensity;
        
        float NoH = clamp(dot(n,h),0.0,1.0);
        float VoH = clamp(dot(v,h),0.0,1.0);
        

        float ndf = SpecularD(NoH, roughness * roughness );
        float g =  SpecularG(l, v, h, n, roughness);
        vec3 f = SpecularF(VoH,f0,roughness);

        vec3 numerator = ndf * g * f;
        float denominator = 4.0 * max(dot(n, v), 0.0) * max(dot(n, l), 0.0) + 0.0001;
        
        vec3 specular  = numerator / denominator;
        vec3 kS = f;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;
        float NdotL = max(dot(n, l), 0.0);
        
        NdotL = max(dot(n, l), 0.0);
        vec3 Lo = (kD * baseColor * InvPI + specular) * radiance * NdotL;
        
        if (light.isCastShadow) 
        {
            float shadow = ShadowCalculationSpolight(light.lightSpaceMatrix * fragPos, n, l, i);
            Lo *= (1.0-shadow);
        }
   
        
        outLo += Lo;
    }
    
    return outLo;
}

vec3 ComputePointLight(vec3 baseColor,vec3 fragPos,vec3 v, vec3 n, float roughness, float metallic, vec3 f0)
{
    vec3 outLo = vec3(0.f);

    for (int i = 0; i < nbrOfPointLight; i++)
    {
        PointLightData light = pointLightData[i];
        float distance = length(light.position - fragPos);

        if(distance > light.radius)
        continue;

        vec3 l = normalize(vec3(light.position - fragPos));
        vec3 h = normalize(v + l);

        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = light.color * attenuation * light.intensity;

        float NoH = clamp(dot(n,h),0.0,1.0);
        float VoH = clamp(dot(v,h),0.0,1.0);


        float ndf = SpecularD(NoH, roughness * roughness );
        float g =  SpecularG(l, v, h, n, roughness);
        vec3 f = SpecularF(VoH,f0,roughness);

        vec3 numerator = ndf * g * f;
        float denominator = 4.0 * max(dot(n, v), 0.0) * max(dot(n, l), 0.0) + 0.0001;

        vec3 specular  = numerator / denominator;
        vec3 kS = f;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;
        float NdotL = max(dot(n, l), 0.0);

        
        NdotL = max(dot(n, l), 0.0);
        outLo += (kD * baseColor * InvPI + specular) * radiance * NdotL;
    }

    return outLo;
}




void main()
{
    vec3 fragPos = texture(gPosition, texCoords).rgb;
    vec4 fragPosVec4 = texture(gPosition, texCoords);

    vec3 normal = texture(gNormal, texCoords).rgb;
    vec3 albedo =  pow(texture(gAlbedoSpec, texCoords).rgb,vec3(2.2));
    
    float metallic = texture(gMetallicRoughessReflectance,texCoords).r;
    float roughness = texture(gMetallicRoughessReflectance,texCoords).g;
    float reflectance = texture(gMetallicRoughessReflectance,texCoords).b;

    float ambientOcclusion = texture(gAmbiantOcclusion,texCoords).r;

    vec4 emissiveValue = texture(gEmissive,texCoords);
    
    float emissive = emissiveValue.w;
    vec3 emissiveColor = emissiveValue.rgb;

    vec3 F0 = vec3(0.04);
    F0 = 0.16 * reflectance * reflectance * (1.0 - metallic) + albedo * metallic;
    
    vec3 Lo = vec3(0.0);
    
    // view unit vector
    vec3 v = normalize(cameraPos - fragPos);
    // Surface Normal
    vec3 n = normalize(normal);
    //reflect Vector
    vec3 r = reflect(-v, n); 

    // Incident Light Unit Vector
    vec3 l = normalize(directionalData.direction);
    // half unit vector
    vec3 h = normalize(v + l);
    
    float NoH = clamp(dot(n,h),0.0,1.0);
    float VoH = clamp(dot(v,h),0.0,1.0);

    vec3 radiance = directionalData.color * directionalData.intensity;
    float ndf = SpecularD(NoH, roughness * roughness );
    float g =  SpecularG(l, v, h, n, roughness);
    vec3 f = SpecularF(VoH,F0,roughness);

    vec3 numerator = ndf * g * f;
    float denominator = 4.0 * max(dot(n, v), 0.0) * max(dot(n, l), 0.0) + 0.0001;
    vec3 specular  = numerator / denominator;
    
    vec3 kS = f;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;
    float NdotL = max(dot(n, l), 0.0);
    
    Lo += (kD * albedo * InvPI + specular) * radiance * NdotL;
    
    
    if (directionalData.isDirlightCastShadow)
    {
        float shadow = ShadowCalculation(directionalData.lightSpaceMatrix * fragPosVec4,n,l);
        Lo *= (1.0-shadow);
    }

    Lo += ComputePointLight(albedo, fragPos, v, n, roughness, metallic, F0);
    Lo += ComputeSpotLight(albedo, fragPosVec4, v, n, roughness, metallic, F0);


    vec3 ambient = ComputeIbl(roughness, kD, ambientOcclusion, albedo, n, r, v, f);
    vec3 color = Lo + ambient + (emissiveColor * emissive);
    
    FragColor = vec4(color, 1);
}
