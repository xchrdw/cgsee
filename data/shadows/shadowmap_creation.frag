#version 150 core

out vec4 fragColor;

void main()
{
	float depth = gl_FragCoord.z;
	float dx = dFdx(depth);
	float dy = dFdy(depth);
	
	fragColor = vec4(depth, depth*depth + 0.25 * (dx*dx + dy*dy), 0.0, 1.0);
}