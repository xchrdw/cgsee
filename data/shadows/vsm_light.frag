#version 150 core

uniform float znear;
uniform float zfar;
uniform int useLayer;

in vec3 normal;

out vec4 fragColor0;
out vec4 fragColor1;
out vec4 fragColor2;
out vec4 fragColor3;

float linearize(float depth) 
{
	return - znear * depth / (zfar * depth - zfar - znear * depth);
}

void main()
{
	float depth = linearize(gl_FragCoord.z);
	float dx = dFdx(depth);
	float dy = dFdy(depth);
	
	if (useLayer == 0){
		fragColor0 = vec4(depth, depth*depth + 0.25 * (dx*dx + dy*dy), 0.0, 1.0);
	} else if (useLayer == 1){
		fragColor1 = vec4(depth, depth*depth + 0.25 * (dx*dx + dy*dy), 0.0, 1.0);
	}
}