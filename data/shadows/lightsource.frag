#version 150 core

in vec4 gl_FragCoord;
out vec4 fragColor;


uniform float znear;
uniform float zfar;

void main()
{
	float z = gl_FragCoord.z; 

	// linear z
	z = - znear * z / (zfar * z - zfar - znear * z);
	
	fragColor = vec4(z);
}
