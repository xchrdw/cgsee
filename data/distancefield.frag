#version 150 core

in vec2 v_uv;

in vec4 gl_FragCoord;
out vec4 fragColor;

uniform sampler2D distancefield;
uniform float pixelWidth;

const float smoothing = 0.1;
const vec3 textColor = vec3(0);
const int charCount = 22;


in flat int v_length;
in flat int v_chars[16];

void main()
{
	int charIndex = int(v_uv.x * v_length);
	vec2 charMapUV = vec2((fract(v_uv.x * v_length) + v_chars[charIndex]) / charCount, v_uv.y);
	float d = 1-texture(distancefield, charMapUV).x;
	float a = smoothstep(0.5 - smoothing, 0.5 + smoothing, d);
	fragColor = vec4(textColor,a*smoothstep(40,60,pixelWidth));
}
