#version 150 core

in vec2 v_uv;
out vec4 fragcolor;

uniform sampler2DMS g_normalDepth;
uniform sampler2D x_edges;
uniform sampler2D unsharpMaskingDepthBuffer;

vec4 texture2DMS(sampler2DMS tex, vec2 coord);

void main()
{
    fragcolor = vec4(texture2DMS(g_normalDepth, gl_FragCoord.xy).rgb, 1.0);
	fragcolor *= vec4(texture2D(x_edges, v_uv).rgb, 1.0);
	fragcolor *= texture2D(unsharpMaskingDepthBuffer, v_uv);
}
