#version 150 core

in vec2 v_uv;

in vec4 gl_FragCoord;
out vec4 fragColor;

uniform sampler2D distancefield;
uniform float pixelWidth;

const vec3 textColor = vec3(0);
const int charCount = 22;


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
	for(int i = 0; i < v_chars.length(); ++i) {
		if(i == charIndex)
			char = v_chars[i];
	}
	vec2 charMapUV = vec2((fract(v_uv.x * v_length) + char) / charCount, v_uv.y);
	float d = 1-texture(distancefield, charMapUV).x;
	float a = aastep(0.5,d);
	fragColor = vec4(textColor,a*smoothstep(40,60,pixelWidth));
}
