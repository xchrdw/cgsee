#version 150 core

in vec4 gl_FragCoord;
out vec4 fragColor;

in vec2 v_uv;

uniform sampler2D image;
uniform float zoom;
uniform vec2 pan;
uniform vec2 aspect;
uniform vec2 viewport;

const vec4 gridColor1 = vec4(vec3(0.7), 1); // dark gray
const vec4 gridColor2 = vec4(vec3(0.9), 1); // light gray

void main()
{
	vec2 position = (v_uv/zoom+pan)/aspect;
	vec4 imageColor = texture(image, position);
	float gridPixelXCount = 200;
	float gridPixelYCount = gridPixelXCount * viewport.y / viewport.x;
	float gridStatus = mod(int(v_uv.x * gridPixelXCount) + int(v_uv.y * gridPixelYCount), 2);
	vec4 gridColor = mix(gridColor1, gridColor2, gridStatus);
	fragColor = vec4(mix(gridColor, imageColor, imageColor.a).xyz, 1);
}
