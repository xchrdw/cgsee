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
// uniform sampler2D testTex;

uniform int randomInt;
uniform int frameCounter;

vec3 light = vec3(0.0, 0.0, -1000.0);
//vec3 cameraposition = vec3(1.0, 0.0, 3.0);
float EPSILON = 0.000001;

void rayTriangleIntersection(vec3 origin, vec3 direction, out int nearestIndex, out vec3 triangle[3], out vec3 intersectionPoint);
vec3 getNormalAndTangentSpaceForTriangle(vec3 triangle[3], out mat3 tangentspace);
float getLight(vec3 pos, vec3 normal);
vec4 skybox(vec3 direction);


float rand =  fract(sin(dot(normalize(direction.xy) ,vec2(12.9898, 78.233)) * (randomInt%1111)) * 43758.5453);

void main()
{
    vec4 oldFragColor = texture(accumulation, v_uv);
    vec4 addedColor = vec4(0.0);
    
    
    int numRnd = textureSize(randomVectors);
    vec3 rndVec = texelFetch(randomVectors, int(rand*numRnd)).xyz;


    vec3 origin = cameraposition;
    vec3 ray = direction;

    for (int i = 0; i < 2; ++i) {
        int primaryNearestIndex;
        vec3 primaryTriangle[3];
        vec3 primaryIntersectionPoint;

        rayTriangleIntersection(origin, ray, primaryNearestIndex, primaryTriangle, primaryIntersectionPoint);

        if (primaryNearestIndex == -1) {
            addedColor += skybox(ray) / (i + 1);
            break;
        }

        mat3 primaryTangentspace;
        vec3 primaryNormalAvg = getNormalAndTangentSpaceForTriangle(primaryTriangle, primaryTangentspace);


        //cornellbox has broken triangle orientations, so use these 
        vec3 normals[3];
        normals[0] = texelFetch(normalBuffer, texelFetch(indexBuffer, primaryNearestIndex+0).x).xyz;
        normals[1] = texelFetch(normalBuffer, texelFetch(indexBuffer, primaryNearestIndex+1).x).xyz;
        normals[2] = texelFetch(normalBuffer, texelFetch(indexBuffer, primaryNearestIndex+2).x).xyz;
        primaryNormalAvg =  (normals[0] + normals[1] + normals[2]) / 3;

        //check the light
        float primaryLight = getLight(primaryIntersectionPoint, primaryNormalAvg);

        addedColor += primaryLight / (i + 1);

        origin = primaryIntersectionPoint;
        ray = normalize(primaryTangentspace * rndVec);
    }

    
    fragColor = mix(oldFragColor, addedColor, 1.0/frameCounter);
    return;
}

float getLight(vec3 pos, vec3 normal) {
    int lightNearestIndex;
    vec3 lightTriangle[3];
    vec3 lightIntersectionPoint;

    rayTriangleIntersection(pos, light - pos, lightNearestIndex, lightTriangle, lightIntersectionPoint);

    float cos = 0.0;
    if (lightNearestIndex == -1) {
        cos = dot(normalize(light), normalize(normal));
    }

    return cos;
}


void rayTriangleIntersection(vec3 origin, vec3 direction, out int nearestIndex, out vec3 triangle[3], out vec3 intersectionPoint) {
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
            triangle[0] = v0;
            triangle[1] = v1;
            triangle[2] = v2;
        }
    }

    intersectionPoint = origin -(direction*0.001) + distanceOfNearest * direction;
}

vec4 skybox(vec3 direction) {
    return vec4(0.09, 0.6, 0.9, 1.0);
}

vec3 getNormalAndTangentSpaceForTriangle(vec3 triangle[3], out mat3 tangentspace) {
    vec3 e0 = triangle[1] - triangle[0];
    vec3 e1 = triangle[2] - triangle[0];

    tangentspace[0] = normalize(e0);
    tangentspace[1] = normalize(cross(e0, e1));
    tangentspace[2] = normalize(cross(tangentspace[0], tangentspace[1]));

    return tangentspace[1];
}