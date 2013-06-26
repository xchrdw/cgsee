// #version 410
#version 150 core

// in vec3 normal;
// in vec3 position;
// in vec3 objCoord;

in vec2 v_uv; //[0,1]*[0,1]

out vec4 fragColor;

uniform vec3 cameraposition;

uniform ivec2 viewport;

uniform mat4 transform;

uniform isamplerBuffer indexBuffer;
uniform samplerBuffer vertexBuffer;
uniform samplerBuffer normalBuffer;
// uniform samplerBuffer geometryBuffer;

void main()
{
    int numIndices  = textureSize(indexBuffer);
    int numVertices = textureSize(vertexBuffer);
    
    int iIndex = int(numIndices  * v_uv.s * v_uv.t);
    int vIndex = int(numVertices * v_uv.s * v_uv.t);
    
    // Check if vertex buffer has the right size (~507)
    // fragColor   = vec4(numVertices / 600.0, 0.0, 0.0, 1.0);
    
    // print index texture, works
    fragColor = texelFetch(indexBuffer,  iIndex)/float(numVertices);
    
    // print vertex/normal Buffer - contain only 0 when index buffer is loaded
    // fragColor = texelFetch(vertexBuffer,  texelFetch(indexBuffer, iIndex).x);
    // fragColor = texelFetch(vertexBuffer, vIndex);
    // fragColor = texelFetch(normalBuffer, vIndex);
   
}
