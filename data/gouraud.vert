#version 330


//partly from http://www.opengl.org/sdk/docs/tutorials/TyphoonLabs/Chapter_4.pdf

in vec3 a_vertex;
in vec3 a_normal;
out vec4 color;

uniform mat4 transform;

uniform float znear;
uniform float zfar;

uniform vec3 cameraposition;
uniform vec3 lightdir;
uniform vec3 attCoeff;

uniform mat4 light;
uniform mat4 material;
uniform float lightshininess;
uniform vec4 lightambientglobal;

void main()
{
	gl_Position = transform * vec4(a_vertex, 1.0);
	vec3 n = normalize(a_normal);
	n *= 0.5;
	n += 0.5;

	float z = gl_Position.z; 

	// d = (2.0 * zfar * znear / (zfar + znear - (zfar - znear) * (2.0 * z- 1.0)));
	// normalized to [0,1]
	// d = (d - znear) / (zfar - znear);

	// simplyfied with wolfram alpha
	z = - znear * z / (zfar * z - zfar - znear * z);

	//light uniforms
	vec4 iAmbient = light[0];
	vec4 iDiffuse = light[1];
	vec4 iSpecular = light[2];
	float shininess = lightshininess;

	//light and camera -> uniforms
	vec3 lightdir = lightdir;		// warum das auch immer nötig ist  
	vec4 specular;				

	//calculate dist, etc
	float dist = length(lightdir); 
	float attenuation = min(1/(attCoeff.x+attCoeff.y*dist+attCoeff.z*dist*dist),1);
	lightdir = normalize(lightdir);
	float nxDir = max(0.0,dot(n,lightdir));
	vec4 diffuse = iDiffuse * nxDir * attenuation;

	if(nxDir != 0.0)
	{
		vec3 cameraVector = normalize(cameraposition - gl_Position.xyz);
		vec3 halfVector = normalize(lightdir + cameraVector);
		float nxHalf = max(0.0,dot(n,halfVector));
		float specularPower = pow(nxHalf, shininess);
		specular = iSpecular * specularPower * attenuation;
	}
					//global ambient			//emission			//local ambient		//diffuse					//specular
	color = lightambientglobal*material[0] + material[3] + iAmbient*material[0] + (diffuse * material[1]) + ( specular * material[2]);

}
