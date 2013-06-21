#version 150 core

// in vec3 normal;
// in vec3 position;
// in vec3 objCoord;

in vec2 v_uv; //[0,1]*[0,1]

out vec4 fragColor;

uniform vec3 cameraposition;

uniform ivec2 viewport;

uniform mat4 transform;

uniform samplerBuffer vertexBuffer;
uniform samplerBuffer normalBuffer;
uniform samplerBuffer geometryBuffer;

void main()
{
    int numVertices = textureSize(vertexBuffer);
    vec3 nearest = vec3(1.0, 1.0, 0.0);
    float minDist = 10000.0;
    float dist = 0.0;
    vec3 vertex = vec3(0.0);
    
    // vec3 objCoord = vec3(viewport * v_uv, 0.0);
    
    // --- do pathtracing --- //
    
    /* int i = 0;
    int nearestIndex = -1;
    for (i = 0; i < numVertices; ++i) {
    	vertex = texelFetch(vertexBuffer, i).xyz;
    	dist = abs(length(vertex - objCoord));
    	if (dist < minDist) {
    		minDist = dist;
    		nearest = vertex;
            nearestIndex = i;
    	}
    } */

    //fragColor = vec4(vec3(nearestIndex/507.0), 1.0);
    //fragColor = texelFetch(normalBuffer, nearestIndex);
    //fragColor = vec4(vec3(normal), 1.0);
    
    fragColor = vec4(v_uv, 0.0, 1.0);
}
