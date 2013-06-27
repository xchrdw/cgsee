#version 150 core

// source http://blog.evoserv.at/index.php/2012/12/hemispherical-screen-space-ambient-occlusion-ssao-for-deferred-renderers-using-openglglsl/

uniform sampler2D normalz;

out vec4 fragcolor;
in vec2 v_uv;


const int sample_count = 16;
const vec2 poisson16[] = vec2[](    // These are the Poisson Disk Samples
    vec2( -0.94201624,  -0.39906216 ),
    vec2(  0.94558609,  -0.76890725 ),
    vec2( -0.094184101, -0.92938870 ),
    vec2(  0.34495938,   0.29387760 ),
    vec2( -0.91588581,   0.45771432 ),
    vec2( -0.81544232,  -0.87912464 ),
    vec2( -0.38277543,   0.27676845 ),
    vec2(  0.97484398,   0.75648379 ),
    vec2(  0.44323325,  -0.97511554 ),
    vec2(  0.53742981,  -0.47373420 ),
    vec2( -0.26496911,  -0.41893023 ),
    vec2(  0.79197514,   0.19090188 ),
    vec2( -0.24188840,   0.99706507 ),
    vec2( -0.81409955,   0.91437590 ),
    vec2(  0.19984126,   0.78641367 ),
    vec2(  0.14383161,  -0.14100790 )
   );

const float filterRadius = 0.02;
const float distanceThreshold = 0.015;


void main()
{
	vec4 normalz_texel = texture(normalz, v_uv);
    
    float depth = normalz_texel.a;
    
    if(depth == 1.0) {
        // discard
        fragcolor = vec4(1.0, 0.0, 1.0, 1.0);
        return;
    }

    vec3 viewPos = vec3(v_uv, depth);
    vec3 viewNormal = normalize(normalz_texel.xyz * 2.0 - 1.0);
 
    float ambientOcclusion = 0;
    // perform AO
    for (int i = 0; i < sample_count; ++i)
    {
        //int i=4;
        vec2 sampleTexCoord = v_uv + (poisson16[i] * (filterRadius));
        float sampleDepth = texture(normalz, sampleTexCoord).a;
        vec3 samplePos = vec3(sampleTexCoord, sampleDepth);
        vec3 sampleDir = normalize(samplePos - viewPos);
 
        // angle between SURFACE-NORMAL and SAMPLE-DIRECTION (vector from SURFACE-POSITION to SAMPLE-POSITION)
        float NdotS = max(dot(viewNormal, sampleDir), 0);
        // distance between SURFACE-POSITION and SAMPLE-POSITION
        float VPdistSP = distance(viewPos, samplePos);
 
        // a = distance function
        float a = 1.0 - smoothstep(distanceThreshold, distanceThreshold * 2, VPdistSP);
        // b = dot-Product
        float b = NdotS;
        
        if(NdotS > 0.0) {
            // discard
            //fragcolor = vec4(0.0, 1.0, 1.0, 1.0);
            //return;
        }
        ambientOcclusion += (a * b);
        
    }

    fragcolor = vec4(1.0 - (ambientOcclusion / sample_count));
}