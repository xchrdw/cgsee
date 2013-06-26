#version 330

in vec4 gl_FragCoord;
in vec4 shadowCoord;

out vec4 fragColor;

uniform float znear;
uniform float zfar;

uniform sampler2D shadowMap;

void main()
{
	vec4 shadowCoordinateWdivide = shadowCoord / shadowCoord.w;
    
    // Used to lower moiré pattern and self-shadowing
    float z = shadowCoordinateWdivide.z - 0.0005;
    z = - znear * z / (zfar * z - zfar - znear * z);
    
    float distanceFromLight = texture(shadowMap,shadowCoordinateWdivide.st).x;
        
    float shadow = 1.0;
    if (shadowCoord.w > 0.0)
        shadow = distanceFromLight < z ? 0.5 : 1.0;
        
    //gl_FragColor = vec4(vec3(z), 1.0);
    //gl_FragColor = vec4(vec3(distanceFromLight), 1.0);
    gl_FragColor = vec4(vec3(shadow), 1.0);
}
