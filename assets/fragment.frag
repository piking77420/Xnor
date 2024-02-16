#version 460 core

out vec4 FragColor;
in vec3 normal;
in vec2 TexCoord;
uniform sampler2D texture1;

in vec3 FragPos;

const vec3 lightPos = vec3(0, 0, 1);
const vec4 color = vec4(0, 1, 0, 1);

void main()
{
    vec4 ambiant = texture(texture1, TexCoord);
    
    vec3 lightDir = normalize(lightPos - FragPos  );
    float diffuse = max(dot(normal, lightDir), 0.0f);

    ambiant += diffuse * color;
    
    FragColor = ambiant;
}
