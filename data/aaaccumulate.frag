#version 150 core

uniform sampler2D inputBuffer;
uniform sampler2D accumulateBuffer;
uniform int accumulatedSubpixels;

out vec4 fragcolor;
in vec2 v_uv;

void main()
{
    //vec3 color = texture(accumulateBuffer, v_uv).rgb + vec3(0.01);
    //fragcolor = vec4(color, 1.0);
    fragcolor = mix(texture(accumulateBuffer, v_uv), texture(inputBuffer, v_uv), 1.0 / accumulatedSubpixels);
    //fragcolor = texture(inputBuffer, v_uv);
    //fragcolor = vec4(vec3(accumulatedSubpixels / 16.0),1.0);
}
