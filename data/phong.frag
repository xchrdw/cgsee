#version 330


//partly from http://www.opengl.org/sdk/docs/tutorials/TyphoonLabs/Chapter_4.pdf

in vec4 gl_FragCoord;
out vec4 gl_FragColor;

in vec3 normal;

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
	vec3 n = normalize(normal);
	n *= 0.5;
	n += 0.5;

	float z = gl_FragCoord.z; 

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
	vec4 specular=vec4(0.0);				

	//calculate dist, etc
	float dist = length(lightdir); 
	float attenuation = min(1/(attCoeff.x+attCoeff.y*dist+attCoeff.z*dist*dist),1);
	lightdir = normalize(lightdir);
	float nxDir = max(0.0,dot(n,lightdir));
	vec4 diffuse = iDiffuse * nxDir * attenuation;

	if(nxDir != 0.0)
	{
		vec3 cameraVector = normalize(cameraposition - gl_FragCoord.xyz);
		vec3 halfVector = normalize(lightdir + cameraVector);
		float nxHalf = max(0.0,dot(n,halfVector));
		float specularPower = max(pow(nxHalf, shininess),0.0);
		specular = iSpecular * specularPower * attenuation;
	}
	//if(nxDir != 0.0)
	//{
					//global ambient			//emission			//local ambient		//diffuse					//specular
	gl_FragColor = lightambientglobal*material[0] + material[3] + iAmbient*material[0] + (diffuse * material[1]) + ( specular * material[2]);
	//}
	//else
	//{
	//	gl_FragColor=vec4(0.0,0.1,0.0,1.0);
	//}
}
