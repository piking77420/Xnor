#version 460 core


layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

layout (std140, binding = 0) uniform CameraUniform
{
    mat4 view;
    mat4 projection;
    vec3 cameraPos;
};
layout (std140, binding = 1) uniform ModelUniform
{
    mat4 model;
    mat4 normalInvertMatrix;
    uint drawId;
};

layout (std140 , binding = 4) uniform MaterialDataUniform
{
	int hasAlbedoMap;
    int hasNormalmap;

};

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords; 
    mat3 TBN;
} vs_out;


void main()
{
    vec4 pos = model * vec4(aPos, 1.0);
    vs_out.FragPos = vec3(pos);
    vs_out.TexCoords = aTexCoords;
    gl_Position =  projection * view * model * vec4(aPos, 1.0);

    // Compute Normal

    if(hasNormalmap == 0)
    {
        vs_out.Normal = mat3(normalInvertMatrix) * aNormal;
    }
    else
    {

       vec3 T = normalize(vec3(model * vec4(aTangent,   0.0f)));
       vec3 B = normalize(vec3(model * vec4(aBitangent, 0.0f)));
       vec3 N = normalize(vec3(model * vec4(aNormal,    0.0f)));
       vs_out.TBN =  mat3(T, B, N);
    }

}
