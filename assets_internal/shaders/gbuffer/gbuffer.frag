#version 460 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

struct Material
{
    sampler2D albedo;
    sampler2D normalMap;
};

layout (std140, binding = 4) uniform MaterialDataUniform
{
	int hasAlbedoMap;
    int hasNormalmap;
};

in VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoords;
    mat3 Tbn;
} fs_in;

uniform Material material;

void main()
{
    gPosition = fs_in.fragPos;

    if (hasNormalmap == 0)
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

    gAlbedoSpec.rgb = texture(material.albedo, fs_in.texCoords).rgb;
}
