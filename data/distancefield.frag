#version 150 core

in vec2 v_uv;
in flat int v_char;

in vec4 gl_FragCoord;
out vec4 fragColor;

uniform sampler2D distancefield;
uniform float pixelWidth;

const float smoothing = 0.1;
const vec3 textColor = vec3(0);
const int charCount = 22;

void main()
{
	float d = 1-texture(distancefield, vec2((v_uv.x + v_char) / charCount, v_uv.y)).x;
	float a = smoothstep(0.5 - smoothing, 0.5 + smoothing, d);
	fragColor = vec4(textColor,a*smoothstep(40,60,pixelWidth));
}
