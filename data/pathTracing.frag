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

uniform int randomInt0;
uniform int randomInt1;
uniform int frameCounter;

vec3 light = vec3(0.0, 0.0, .0);
vec3 arealight[4] = vec3[]( vec3(343.0, 547.8, 227.0),
        vec3(343.0, 547.8, 282.0),
        vec3(213.0, 547.8, 332.0),
        vec3(213.0, 547.8, 227.0));
//vec3 cameraposition = vec3(1.0, 0.0, 3.0);
float EPSILON = 0.000001;

void rayTriangleIntersection(vec3 origin, vec3 direction, out int nearestIndex, out vec3 triangle[3], out vec3 intersectionPoint);
bool rayTriangleIntersectionBool(vec3 origin, vec3 target);
vec3 getNormalAndTangentSpaceForTriangle(vec3 triangle[3], out mat3 tangentspace);
float getLight(vec3 pos, vec3 normal);
vec4 skybox(vec3 direction);


float rand =  fract(sin(dot(normalize(direction.xy) ,vec2(12.9898, 78.233)) * (randomInt0%1111)) * 43758.5453);

float rand2 =  fract(sin(dot(normalize(direction.xy) ,vec2(12.9898, 78.233)) * (randomInt1%1111)) * 43758.5453); // :)

void main()
{
    vec4 oldFragColor = texture(accumulation, v_uv);
    vec4 addedColor = vec4(0.0);
    
    
    int numRnd = textureSize(randomVectors);
    vec3 rndVec = texelFetch(randomVectors, int(rand*numRnd)).xyz;


    vec3 origin = cameraposition;
    vec3 ray = direction;

    for (int i = 0; i < 3; ++i) {
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

        //check the light
        float primaryLight = getLight(primaryIntersectionPoint, primaryNormalAvg);

        addedColor += primaryLight / (i + 1);

        origin = primaryIntersectionPoint;
        ray = normalize(primaryTangentspace * rndVec);
    }

    
    fragColor = mix(oldFragColor, addedColor, 1.0/frameCounter);

    //fragColor = vec4(0.1 + 0.0013 * vec3(texelFetch(geometryBuffer, 57).xyz), 0.5);

    return;
}

float getLight(vec3 pos, vec3 normal) {
    vec3 randomLightPos = (mix(arealight[2], arealight[3], rand2) + mix(arealight[0], arealight[1], rand)) / 2;

    bool lightShines = rayTriangleIntersectionBool(pos, randomLightPos);

    float cos = 0.0;
    if (lightShines) {
        cos = dot(normalize(randomLightPos - pos), normalize(normal))*0.6;
    }

    return cos;
}


bool rayTriangleIntersectionBool(vec3 origin, vec3 target) {
    vec3 direction = target - origin;

    int nearestIndex;
    vec3 triangle[3];
    vec3 intersectionPoint;

    rayTriangleIntersection(origin, direction, nearestIndex, triangle, intersectionPoint);

    return length(target-origin) < length(intersectionPoint-origin);
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

    vec3 normal = normalize(cross(e0, e1));
    vec3 anotherone = normalize(vec3(rand+0.5, rand, rand));


    tangentspace[0] = cross(normal, anotherone);
    tangentspace[1] = normal;
    tangentspace[2] = (cross(tangentspace[0], tangentspace[1]));

    return tangentspace[1];
}