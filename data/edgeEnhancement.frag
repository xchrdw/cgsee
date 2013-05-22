#version 150 core

in vec2 v_uv;
out vec4 fragcolor;

uniform sampler2DMS g_normalDepth;

vec4 texture2DMSSample(sampler2DMS tex, vec2 coord, int sample);

#define NUM_SAMPLES 16

void main (void) {
    // Texture Center and Offset
    vec2 center = gl_FragCoord.xy;
    vec2 off    = vec2(1.0);

    vec4 tex_nw = vec4(center.x+off.x, center.y+off.y, 1.0, 1.0);
    vec4 tex_ne = vec4(center.x-off.x, center.y+off.y, 1.0, 1.0);
    vec4 tex_sw = vec4(center.x+off.x, center.y-off.y, 1.0, 1.0);
    vec4 tex_se = vec4(center.x-off.x, center.y-off.y, 1.0, 1.0);
	
	vec3 d = vec3(0.0);
    for(int i=0; i < NUM_SAMPLES; i++) {
	    vec4 val_nw = texture2DMSSample(g_normalDepth, tex_nw.xy, i);
		val_nw.xyz = (val_nw.xyz*2.0)-1.0;
		
        vec4 val_ne = texture2DMSSample(g_normalDepth, tex_ne.xy, i);
		val_ne.xyz = (val_ne.xyz*2.0)-1.0;
		
        vec4 val_sw = texture2DMSSample(g_normalDepth, tex_sw.xy, i);
		val_sw.xyz = (val_sw.xyz*2.0)-1.0;
		
        vec4 val_se = texture2DMSSample(g_normalDepth, tex_se.xy, i);
		val_se.xyz = (val_se.xyz*2.0)-1.0;
		
		// Calculate discontinuities
		vec3 discontinuity = vec3(0.0,0.0,0.0);
		
		// (north west DOT south east) AND (north east DOT south west)
		float dot0 = dot(val_nw.xyz, val_se.xyz);
		float dot1 = dot(val_ne.xyz, val_sw.xyz);
		discontinuity.x = 0.5*(dot0+dot1);
		
		// (north west DEPTH DISCONT. south east) AND (north east DEPTH DISCONT. south west)
		float depth_discont0 = 1.0-abs(val_nw.w - val_se.w);
		float depth_discont1 = 1.0-abs(val_ne.w - val_sw.w);
		discontinuity.y = depth_discont0*depth_discont1; // 1.0;  // Uncomment to remove edges in depth discontinuities
		
		discontinuity.z = discontinuity.x*discontinuity.y;
		
		d += clamp(discontinuity, vec3(0.0), vec3(1.0));
	}

    d = d / NUM_SAMPLES;

    fragcolor = vec4(vec3(d.z), 1.0);
	
    return;
}
