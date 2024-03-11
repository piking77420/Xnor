#version 460 core
#extension GL_NV_gpu_shader5 : require

out vec4 FragColor;

in vec2 texCoords;

uniform sampler2D bufferTextureId;

void main()
{
    vec3 color = texture(bufferTextureId, texCoords).rgb;

    FragColor = vec4(color,1.0);
}
