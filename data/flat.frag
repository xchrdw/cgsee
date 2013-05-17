#version 330


//partly from http://www.opengl.org/sdk/docs/tutorials/TyphoonLabs/Chapter_4.pdf

in vec4 gl_FragCoord;
out vec4 gl_FragColor;

in vec3 normal;

uniform float znear;
uniform float zfar;

uniform vec3 cameraposition;
uniform vec3 lightdir;
uniform vec3 lightdir2;
uniform vec3 attCoeff;
uniform vec3 attCoeff2;

uniform mat4 light;
uniform mat4 light2;
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
	vec4 iAmbient[2];
	iAmbient[0] = light[0];
	iAmbient[1] = light2[0];
	vec4 iDiffuse[2];
	iDiffuse[0] = light[1];
	iDiffuse[1] = light2[1];
	vec4 iSpecular[2];
	iSpecular[0]=light[2];
	iSpecular[1]=light2[2];
	float shininess = lightshininess;

	//light and camera -> uniforms
	//vec3 lightdir = lightdir;		// warum das auch immer nötig ist  
	vec4 specular[2];
	for(int i=0; i<2; i++){
		specular[i]=vec4(0.0);}

	//calculate dist, etc
	vec3 lightdirection[2];
	lightdirection[0]=lightdir;
	lightdirection[1]=lightdir2;
	float dist[2];
	dist[0] = length(lightdir);
	dist[1] = length(lightdir2); 
	vec3 att[2];
	att[0]=attCoeff;
	att[1]=attCoeff2;
	float attenuation[2];
	float nxDir[2];

	for(int i=0; i<2; i++){
		 attenuation[i]= min(1/(att[i].x+att[i].y*dist[i]+att[i].z*dist[i]*dist[i]),1);
		lightdirection[i] = normalize(lightdirection[i]);
		nxDir[i] = max(0.0,dot(n,lightdirection[i]));}
	vec4 diffuse[2];
	for(int i=0; i<2; i++){
		diffuse[i] = iDiffuse[i] * nxDir[i] * attenuation[i];}

for(int i=0; i<2; i++)
{	if(nxDir[i] != 0.0)
	{
		vec3 cameraVector = normalize(cameraposition - gl_FragCoord.xyz);
		vec3 halfVector = normalize(lightdirection[i] + cameraVector);
		float nxHalf = max(0.0,dot(n,halfVector));
		float specularPower = max(pow(nxHalf, shininess),0.0);
		specular[i] = iSpecular[i] * specularPower * attenuation[i];
	}
	}
					//global ambient			//emission
	vec4 color=lightambientglobal*material[0] + material[3];
	for(int i=0; i<2;i++)
	{
							//local ambient			//diffuse					//specular
		color += color + iAmbient[i]*material[0] + attenuation[i]*(diffuse[i] * material[1] + specular[i] * material[2]);
	}

	//gl_FragColor=color;

	gl_FragColor=vec4(normal, 1.0);
}