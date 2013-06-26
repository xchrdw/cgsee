#version 330

in vec4 gl_FragCoord;
out vec4 fragColor;



uniform float znear;
uniform float zfar;

void main()
{
	float z = gl_FragCoord.z; 

	//float d = (2.0 * zfar * znear / (zfar + znear - (zfar - znear) * (2.0 * z- 1.0)));
	// normalized to [0,1]
	//d = (d - znear) / (zfar - znear);

	// simplyfied with wolfram alpha
	z = - znear * z / (zfar * z - zfar - znear * z);
	
	fragColor = vec4(z);
}
