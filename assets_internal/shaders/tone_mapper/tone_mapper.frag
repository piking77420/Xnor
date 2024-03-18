#version 460 core

out vec4 fragColor;

in vec2 texCoords;
uniform sampler2D beforeToneMappedImage;
uniform sampler2D bloomBlur;

// linear to tonemapped
vec3 Aces(vec3 x)
{
    // https://knarkowicz.wordpress.com/2016/01/06/aces-filmic-tone-mapping-curve/
    return x * (2.51 * x + 0.03) / (x * (2.43 * x + 0.59) + 0.14);
}

// tonemapped to linear
vec3 AcesInv(vec3 x)
{
    // thanks to https://www.wolframalpha.com/input?i=2.51y%5E2%2B.03y%3Dx%282.43y%5E2%2B.59y%2B.14%29+solve+for+y
    return (sqrt(-10127.0 * x * x + 13702.0 * x + 9.0) + 59.0 * x - 3.0) / (502.0 - 486.0 * x);
}

vec3 ChangeExposure(vec3 col, vec3 b)
{
    b *= col;
    return b / (b - col + 1.0);
}

vec3 ChangeExposure(vec3 col, float b)
{
    return ChangeExposure(col, vec3(b));
}

void main()
{    
    const float gamma = 2.2;
	vec3 bloomColor = texture(bloomBlur,texCoords).rgb;
	vec3 hdrImage = texture(beforeToneMappedImage,texCoords).rgb;

    vec3 color = mix(hdrImage, bloomColor, 0.1f);
    // tone mapping
    color = color / (color + vec3(1.0));
    // gamma correction
    color = pow(color, vec3(1.0/gamma));
    fragColor = vec4(color, 1.0);

    
    // Test power down // 
    
    //vec3 hdrColor = texture(hdrImage, texCoords).rgb;
    //fragColor = vec4(ChangeExposure(Aces(hdrColor), 1.0), 1.0);
    //fragColor = vec4(pow(vec3(fragColor), vec3(1.0/2.2)),1.0);
    
    //

}
