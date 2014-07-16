#version 150 core

in vec2 v_uv;

in vec4 gl_FragCoord;
out vec4 fragColor;

uniform sampler2D distancefield;
uniform float pixelWidth;

const int charCount = 22;

in float v_gray;

in flat int v_length;
in flat int v_chars[16];

float aastep(float threshold, float value)
{
	float afwidth = length(vec2(dFdx( value ) , dFdy( value ))) ;
	//GLSL's fwidth(value) is abs(dFdx(value)) + abs(dFdy(value))
	return smoothstep( threshold - afwidth , threshold + afwidth , value ) ;
}

void main()
{
	int charIndex = int(v_uv.x * v_length);
	int char = 0;
	// We need to use a iteration variable to access the array, due to a
	// bug/feature of some drivers (eg Intel HD 4000)
	for(int i = charIndex; i == charIndex; ++i) {
			char = v_chars[i];
	}
	vec2 charMapUV = vec2((fract(v_uv.x * v_length) + char) / charCount, v_uv.y);
	float d = 1-texture(distancefield, charMapUV).x;
	float a = aastep(0.5,d);
	vec3 textColor = vec3(fract(v_gray + 0.5));
	fragColor = vec4(textColor,a*smoothstep(70,80,pixelWidth));
}
