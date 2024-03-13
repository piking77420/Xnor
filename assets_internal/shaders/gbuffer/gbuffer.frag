#version 460 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;
layout (location = 3) out vec3 gMetallicRoughessReflectance;
layout (location = 4) out vec2 gEmissiveAmbiantOcclusion;



struct Material
{
    sampler2D albedoMap;
    sampler2D metallicMap;
    sampler2D roughnessMap;
    sampler2D normalMap;
    sampler2D ambiantOcclusionMap;
};

layout (std140, binding = 4) uniform MaterialDataUniform
{
    vec3 AlbedoColor;
    bool hasAlbedoMap;
    bool hasMetallicMap;
    bool hasRoughnessMap;
    bool hasNormalMap;
    bool hasAmbiantOcclusionMap;

    float metallic;
    float roughness;
    float reflectance;
    float emissive;
    float ambiantOccusion;
};

in VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoords;

    float metallic;
    float roughness;
    float reflectance;
    float emissive;
    float ambiantOccusion;
    
    mat3 Tbn;
} fs_in;

uniform Material material;

void main()
{
    gPosition = fs_in.fragPos;

    if (hasAlbedoMap == false)
    {
        gAlbedoSpec.rgb = AlbedoColor;
    }
    else
    {
        gAlbedoSpec.rgb = texture(material.albedoMap, fs_in.texCoords).rgb;
    }

    if (hasMetallicMap == false)
    {
        gMetallicRoughessReflectance.r = metallic;
    }
    else
    {
        gMetallicRoughessReflectance.r = texture(material.metallicMap, fs_in.texCoords).r;
    }
    
    if (hasRoughnessMap == false)
    {
        gMetallicRoughessReflectance.g = roughness;
    }
    else
    {
        gMetallicRoughessReflectance.g = texture(material.roughnessMap, fs_in.texCoords).r;
    }
    
    if (hasNormalMap == false)
    {
        gNormal = normalize(fs_in.normal);
    }
    else
    {
        // Compute NormalMap
        vec3 normal = texture(material.normalMap, fs_in.texCoords).rgb;
        normal = normal  * 2.0f - 1.0f;
        gNormal.rgb = normalize(fs_in.Tbn * normal); 
    }

  

    gMetallicRoughessReflectance = vec3(gMetallicRoughessReflectance.r,gMetallicRoughessReflectance.g,reflectance);
    gEmissiveAmbiantOcclusion = vec2(emissive,ambiantOccusion);
}
