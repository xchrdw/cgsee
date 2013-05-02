#version 330


//partly from http://www.opengl.org/sdk/docs/tutorials/TyphoonLabs/Chapter_4.pdf

in vec4 gl_FragCoord;
out vec4 gl_FragColor;

in vec3 normal;

uniform float znear;
uniform float zfar;

//uniform camera_position;

void main()
{
	vec3 n = normalize(normal);
	n *= 0.5;
	n += 0.5;

	float z = gl_FragCoord.z; 

	// d = (2.0 * zfar * znear / (zfar + znear - (zfar - znear) * (2.0 * z- 1.0)));
	// normalized to [0,1]
	// d = (d - znear) / (zfar - znear);

	// simplyfied with wolfram alpha
	z = - znear * z / (zfar * z - zfar - znear * z);

	vec4 specular = vec4(0.0); 
	vec3 camera_position = vec3(0.3,0.4,0.5);
	vec4 vambient = vec4(1.0,1.0,1.0,1);
	vec4 vdiffuse = vec4(0.3,0.5,0.7,1);
	vec4 vspecular = vec4(0.3,0.5,0.7,1);
	float shininess = 0.4;
	vec3 lightposition = vec3(-9.5,-4.5,9.5);
	vec3 lightVector = lightposition - vec3(0,0,0);
	float dist = length(lightVector); 
	float attenuation = 1.0;
	lightVector = normalize(lightVector);
	float nxDir = max(0.0,dot(n,lightVector));
	vec4 diffuse = vdiffuse * nxDir * attenuation;

	if(nxDir != 0.0)
	{
		vec3 cameraVector = normalize(camera_position - gl_FragCoord.xyz);
		vec3 halfVector = normalize(lightVector + cameraVector);
		float nxHalf = max(0.0,dot(n,halfVector));
		float specularPower = pow(nxHalf, shininess);
		specular = vspecular * specularPower * attenuation;
	}

	
	gl_FragColor = vambient/7 + (diffuse * vec4(0.0,0.0,1.0,1)); //+ ( specular * 1)  ;


	//gl_FragColor = vec4(n, z);
	//gl_FragColor = vec4(0.7,0.7,0.7, z);


}
