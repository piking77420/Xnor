#version 460 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;
layout (location = 3) out vec3 gMetallicRoughessReflectance;
layout (location = 4) out vec2 gEmissiveAmbiantOcclusion;



struct Material
{
    sampler2D albedo;
    sampler2D normalMap;
};

layout (std140, binding = 4) uniform MaterialDataUniform
{
    vec3 AlbedoColor;
    bool hasAlbedoMap;
    bool hasNormalmap;
    
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

    if (hasNormalmap == false)
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

    if (hasAlbedoMap == false)
    {
        gAlbedoSpec.rgb = AlbedoColor;
    }   
    else
    {
        gAlbedoSpec.rgb = AlbedoColor;
        gAlbedoSpec.rgb = texture(material.albedo, fs_in.texCoords).rgb;
    }

    gMetallicRoughessReflectance = vec3(metallic,roughness,reflectance);
    gEmissiveAmbiantOcclusion = vec2(emissive,ambiantOccusion);

}
