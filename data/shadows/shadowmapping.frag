#version 150

uniform ivec2 viewport;
uniform float znear;
uniform float zfar;

//uniform sampler2D source;
uniform sampler2D shadowMap;


in vec4 gl_FragCoord;
in vec4 shadowCoord;

out vec4 fragColor;


void main()
{
    float u = gl_FragCoord.x / viewport.x;
    float v = gl_FragCoord.y / viewport.y;
    
    //vec4 sourceFragment = texture(source, vec2(u,v));

	vec4 shadowCoordinateWdivide = shadowCoord / shadowCoord.w;
    
    // Used to lower moiré pattern and self-shadowing
    float z = shadowCoordinateWdivide.z - 0.0005;
    z = - znear * z / (zfar * z - zfar - znear * z);
    
    float distanceFromLight = texture(shadowMap, shadowCoordinateWdivide.st).z;
        
    float shadow = distanceFromLight < z ? 0.5 : 1.0;
        
    //gl_FragColor = vec4(vec3(z), 1.0);
    //gl_FragColor = vec4(vec3(distanceFromLight), 1.0);
    gl_FragColor = vec4(vec3(shadow), 1.0);
    //gl_FragColor =  sourceFragment;
    /*if (v<0.5)
        gl_FragColor = vec4(u,v,0.0,1.0);
    else
        gl_FragColor = vec4(u,v,1.0,1.0);
      */  
    
}
