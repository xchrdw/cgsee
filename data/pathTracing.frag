#version 150 core

in vec3 normal;
in vec3 position;

out vec4 fragColor;

uniform vec3 cameraposition;

uniform samplerBuffer vertexBuffer;
uniform samplerBuffer normalBuffer;
uniform samplerBuffer geometryBuffer;

void main()
{
	int numVertices = textureSize(vertexBuffer);
	vec3 nearest = vec3(1.0, 0.0, 0.0);
	float minDist = 10000000.0, dist = 0.0;
	vec3 vertex = vec3(0.0);
	
    int i = 0;
    int nearestIndex=-1;
	//for (i = 0; i < numVertices; ++i) {
    for (i = 0; i < 507; ++i) {
		vertex = texelFetch(vertexBuffer, i);
		dist = distance(position, vertex);
		if (dist < minDist) {
			minDist = dist;
			nearest = vertex;
            nearestIndex = i;
		}
	}
    vec3 normal = texelFetch(normalBuffer, nearestIndex);
    fragColor = vec4(normal, 1.0);
}
