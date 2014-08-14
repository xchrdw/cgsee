#version 150 core

out vec4 fragcolor;
in vec2 v_uv;

uniform sampler2D source;
uniform sampler2D shadows;
uniform sampler2D ssao;

float CalcLuminance(vec3 color)
{
    return max(dot(color, vec3(0.299, 0.587, 0.114)), 0.0001);
}

// Applies the filmic curve from John Hable's presentation
vec3 ToneMapFilmicALU(vec3 color)
{
    color = max(vec3(0), color - vec3(0.004));
    color = (color * (6.2 * color + 0.5)) / (color * (6.2 * color + 1.7)+ 0.06);

    // result has 1/2.2 baked in
    return pow(color, vec3(2.2));
}

// Applies Reinhard's basic tone mapping operator
vec3 ToneMapReinhard(vec3 color) 
{
    vec3 LuminanceSaturation = vec3(1.0); // Range [0:4]
    float pixelLuminance = CalcLuminance(color);    
    float toneMappedLuminance = pixelLuminance / (pixelLuminance + 1);
    return toneMappedLuminance * pow(color / pixelLuminance, LuminanceSaturation);    
}

// Logarithmic mapping
vec3 ToneMapLogarithmic(vec3 color)
{
    vec3 LuminanceSaturation = vec3(1.0); // Range [0:4]
    float WhiteLevel = 2.0; // Range [0:25]
    float pixelLuminance = CalcLuminance(color);    
    float toneMappedLuminance = log10(1 + pixelLuminance) / log10(1 + WhiteLevel);
    return toneMappedLuminance * pow(color / pixelLuminance, LuminanceSaturation); 
}

#define ToneMap(c) ToneMapLogarithmic(c)
//#define ToneMap(c) ToneMapFilmicALU(c)
//#define ToneMap(c) ToneMapReinhard(c)


void main()
{
    // if texture(source, v_uv).w == 0, we paint a bounding box,
    // therefore we don't apply the shadowing to that fragment.

    float exposure = 0.0;
    vec4 color = exp2(exposure) * texture(source, v_uv);
    color = vec4(ToneMap(color.xyz), 1.0);

    fragcolor = mix(
        color * min(texture(shadows, v_uv)+0.2, texture(ssao, v_uv)),
        color,
        step(texture(source, v_uv).w, 0.0)
        );
}
