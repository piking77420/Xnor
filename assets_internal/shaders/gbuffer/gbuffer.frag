#version 460 core


layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;


struct Material
{
    sampler2D albedo;
    sampler2D normalMap;
};

layout (std140 , binding = 4) uniform MaterialDataUniform
{
	int hasAlbedoMap;
    int hasNormalmap;
};

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    mat3 TBN;
} fs_in;    


uniform Material material;

void main()
{

    gPosition = fs_in.FragPos;

    if (hasNormalmap == 0)
    {
        gNormal = normalize(fs_in.Normal);
    }
    else
    {
        // Compute NormalMap
        vec3 normal = texture(material.normalMap, fs_in.TexCoords).rgb;
        normal = normal  * 2.0f - 1.0f;   
        gNormal.rgb = normalize(fs_in.TBN * normal); 
    }   

    gAlbedoSpec.rgb = texture(material.albedo, fs_in.TexCoords).rgb;
}
