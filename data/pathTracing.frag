// #version 410
#version 150 core

in vec3 normal;
in vec3 position;
in vec3 objCoord;

in vec2 v_uv; //[0,1]*[0,1]
in vec3 direction;

out vec4 fragColor;

uniform vec3 cameraposition;

uniform ivec2 viewport;

uniform mat4 transform;

uniform isamplerBuffer indexBuffer;
uniform samplerBuffer vertexBuffer;
uniform samplerBuffer normalBuffer;
uniform samplerBuffer geometryBuffer;
uniform samplerBuffer randomVectors;
uniform sampler2D accumulation;


vec3 light = vec3(1000.0, 0.0, 0.0);
//vec3 cameraposition = vec3(1.0, 0.0, 3.0);
float EPSILON = 0.000001;

void rayTriangleIntersection(vec3 origin, vec3 direction, out int nearestIndex, out vec3 intersectionPoint);


float rand =  fract(sin(dot(direction.xy ,vec2(12.9898,78.233))) * 43758.5453);

void main()
{
    vec4 oldFragColor = texture(accumulation, v_uv);
    // int numRnd = textureSize(randomVectors);
    // vec3 rndVec = texelFetch(randomVectors, int(rand*numRnd)).xyz;
    // fragColor = vec4(rndVec, 1.0);
    // return;
    // fragColor = vec4(cameraposition, 1.0);

    int primaryNearestIndex;
    vec3 primaryIntersectionPoint;

    rayTriangleIntersection(cameraposition, direction, primaryNearestIndex, primaryIntersectionPoint);

    if (primaryNearestIndex == -1) {
        fragColor = vec4(0,0,1,0);
        return;
    }

    // fragColor = vec4(primaryIntersectionPoint, 1.0);
    fragColor = mix(vec4(primaryIntersectionPoint, 1.0), oldFragColor, 0.001);
    return;

    int secondaryNearestIndex;
    vec3 secondaryIntersectionPoint;

    rayTriangleIntersection(primaryIntersectionPoint, light - primaryIntersectionPoint, secondaryNearestIndex, secondaryIntersectionPoint);

    if (secondaryNearestIndex != -1) {
        fragColor = vec4(0,0,0,0);
        return;
    }

    int firstVertexIndex = texelFetch(indexBuffer, primaryNearestIndex).x;
    vec3 normalAvg = texelFetch(normalBuffer, texelFetch(indexBuffer, primaryNearestIndex).x).xyz;
    normalAvg += texelFetch(normalBuffer, texelFetch(indexBuffer, primaryNearestIndex+1).x).xyz;
    normalAvg += texelFetch(normalBuffer, texelFetch(indexBuffer, primaryNearestIndex+2).x).xyz;
    normalAvg /= 3.0;

    float cos = dot(normalize(light), normalize(normalAvg));

    fragColor = mix(vec4(vec3(cos), 1.0), oldFragColor, 0.5);

}


void rayTriangleIntersection(vec3 origin, vec3 direction, out int nearestIndex, out vec3 intersectionPoint) {
    int numIndices = textureSize(indexBuffer);
    nearestIndex = -1;
    float distanceOfNearest = 100000.0;

    for (int i = 0; i < numIndices - 2; i += 3) {
        vec3 v0 = texelFetch(vertexBuffer, int(texelFetch(indexBuffer, i + 0).x)).xyz;
        vec3 v1 = texelFetch(vertexBuffer, int(texelFetch(indexBuffer, i + 1).x)).xyz;
        vec3 v2 = texelFetch(vertexBuffer, int(texelFetch(indexBuffer, i + 2).x)).xyz;

        vec3 edge0 = v1 - v0;
        vec3 edge1 = v2 - v0;

        vec3 pvec = cross(direction, edge1);

        float det = dot(edge0, pvec);

        //check parallel
        if (det > -EPSILON && det < EPSILON)
           continue;

        float inv_det = 1.0 / det;

        vec3 tvec = origin - v0;

        float u = dot(tvec, pvec) * inv_det;
        if (u < 0.0 || u > 1.0)
            continue;

        vec3 qvec = cross(tvec, edge0);
        float v  = dot(direction, qvec) * inv_det;
        if (v < 0.0 || (u + v > 1.0))
            continue;

        // found it
        float t = dot (edge1, qvec) * inv_det;

        if (t < distanceOfNearest && t > EPSILON) {
            distanceOfNearest = t;
            nearestIndex = i;
        }
    }

    intersectionPoint = origin + distanceOfNearest * direction;
}