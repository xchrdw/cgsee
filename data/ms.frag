#version 150 core

#define NUM_SAMPLES 16

vec4 texture2DMSSample(sampler2DMS tex, vec2 coord, int sample) {
    return texelFetch(tex, ivec2(coord), sample);
}

vec4 texture2DMS(sampler2DMS tex, vec2 coord) {
    vec4 c = vec4(0.0);

    for(int i=0; i < NUM_SAMPLES; i++) {
        c += texelFetch(tex, ivec2(coord), i);
    }

    c /= NUM_SAMPLES;

    return c;
}
