#version 460 core

out vec4 FragColor;


// linear to tonemapped
vec3 ACES(vec3 x) {
    return x*(2.51*x + .03) / (x*(2.43*x + .59) + .14); // https://knarkowicz.wordpress.com/2016/01/06/aces-filmic-tone-mapping-curve/
}

// tonemapped to linear
vec3 ACES_Inv(vec3 x) {
    return (sqrt(-10127.*x*x + 13702.*x + 9.) + 59.*x - 3.) / (502. - 486.*x); // thanks to https://www.wolframalpha.com/input?i=2.51y%5E2%2B.03y%3Dx%282.43y%5E2%2B.59y%2B.14%29+solve+for+y
}

vec3 changeExposure(vec3 col, vec3 b) {
    b *= col;
    return b/(b-col+1.);
}

vec3 changeExposure(vec3 col, float b) {
    return changeExposure(col, vec3(b));
}


in vec2 TexCoords;
uniform sampler2D beforeToneMappedImage;


void main()
{
    const float gamma = 2.2;
    vec3 hdrColor = texture(beforeToneMappedImage, TexCoords).rgb;
    
     FragColor = vec4(changeExposure(ACES(hdrColor),1.0), 1.0);
   // FragColor = vec4(1,1,1,1.0);
}
    
