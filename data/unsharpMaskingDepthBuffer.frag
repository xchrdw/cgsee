#version 150 core

in vec2 v_uv;
out vec4 fragcolor;

uniform sampler2DMS g_normalDepth;

vec4 texture2DMS(sampler2DMS tex, vec2 coord);

float readDepth(in vec2 coord)
{
    return texture2DMS(g_normalDepth, coord).w;
}

void calculateAO(
    in vec2 texCoord,
    in float threshold,
    in float aoCap,
    in float aoMultiplier,
    in float depth,
    inout float ao,
    inout int pixelsCaculated)
{
    // read depth map
    float d = readDepth(texCoord);
    // if difference of distances below treshold...
    if(abs(d-depth) < threshold) {
        // map to AO amount
        ao += min(aoCap, max(0.0, depth-d) * aoMultiplier);
        // propagate to sum
        pixelsCaculated++;
    }
    return;
}

#define KERNEL_SIZE 16

vec4 unsharpMaskingDepthBuffer()
{	
    vec2 poisson[16];
    poisson[0] = vec2(0.007937789, 0.73124397);
    poisson[1] = vec2(-0.10177308, -0.6509396);
    poisson[2] = vec2(-0.9906806, -0.63400936);
    poisson[3] = vec2(-0.5583586, -0.3614012);
    poisson[4] = vec2(0.7163085, 0.22836149);
    poisson[5] = vec2(-0.65210974, 0.37117887);
    poisson[6] = vec2(-0.12714535, 0.112056136);
    poisson[7] = vec2(0.48898065, -0.66669613);
    poisson[8] = vec2(-0.9744036, 0.9155904);
    poisson[9] = vec2(0.9274436, -0.9896486);
    poisson[10] = vec2(0.9782181, 0.90990245);
    poisson[11] = vec2(0.96427417, -0.25506377);
    poisson[12] = vec2(-0.5021933, -0.9712455);
    poisson[13] = vec2(0.3091557, -0.17652994);
    poisson[14] = vec2(0.4665941, 0.96454906);
    poisson[15] = vec2(-0.461774, 0.9360856);

    vec2 texCoord = gl_FragCoord.xy;

    float ao = 0.0;
    vec2 t = vec2(1.0);
    float threshold = 1.5;

    int pixelsCaculated = 0;
    float aoCap = 0.50;
    float aoMultiplier= 10.0;
    float depth = readDepth( texCoord );

    //if( !(depth == 1)) {
        int iterations = 5;
        int kernelSize = KERNEL_SIZE;
        for(int i = 0; i < iterations; i++) {
            // Iterate over kernel
            for(int j = 0; j < kernelSize; j++) {
                calculateAO(
                    texCoord + poisson[j] * t,
                    threshold,
                    aoCap,
                    aoMultiplier,
                    depth,
                    ao,
                    pixelsCaculated);
            }

            t*=2.0;
            aoMultiplier*= 1.5;
        }

        ao/= float(pixelsCaculated);
    //}

    float valScaled = 1.0 - ao;

    return vec4(valScaled, valScaled, valScaled, 1.0);
}

void main (void) {
	fragcolor = unsharpMaskingDepthBuffer();
    
    return;
}
