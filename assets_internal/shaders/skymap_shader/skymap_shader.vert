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



out VS_OUT {
    vec3 TexCoords;
} vs_out;



void main()
{
    vs_out.TexCoords = aPos;

	mat3 viewMat3 = mat3(view);
 
    vec4 pos = projection * mat4(viewMat3) * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}  
