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
float INFINITY = 1000000000000.0;
int INT_MAX = 2147483647;


// ray datastructure {{{
struct Ray {
    vec3 origin;
    vec3 direction;
    vec3 inv_direction;
    int sign[3];
};

Ray makeRay(in vec3 origin, in vec3 direction) {
    vec3 inv_direction = vec3(1.0) / direction;
    return Ray(
        origin,
        direction,
        inv_direction,
        int[3] ( int(inv_direction.x < 0.0), int(inv_direction.y < 0.0), int(inv_direction.z < 0.0) )
    );
}
// }}} ray datastructure

void rayBVHIntersection(
    in Ray ray, 
    out int nearestIndex, out vec3 triangle[3], out vec3 intersectionPoint,
    out float dist);
float rayBoxIntersectionDistance(
    in Ray ray, 
    in vec3 aabb[2]);
float rayTriangleIntersectionDistance(
    in Ray ray, 
    in vec3 v0, in vec3 v1, in vec3 v2);

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

    Ray currentRay = makeRay(cameraposition, direction);

    //TODO: test {{{
    
    int primaryNearestIndex; //TODO: primaryNearestIndex is not needed if we test e.g. primaryIntersectionPoint.x == INFINITY (has to be set in rayBVHIntersection)
    vec3 primaryTriangle[3];
    vec3 primaryIntersectionPoint;

    float dist;

    for (int i = 0; i < 3; ++i) {

        //TODO: problems in both implementations: some strange rendering artifacts, 
        //different colors for the same object from different angles
        //works for 1 x aabb + triangles
        rayBVHIntersection(currentRay, primaryNearestIndex, primaryTriangle, primaryIntersectionPoint, dist);
        //works:
        //rayTriangleIntersection(currentRay.origin, currentRay.direction, primaryNearestIndex, primaryTriangle, primaryIntersectionPoint);

        if (primaryNearestIndex == -1) {
            addedColor += skybox(currentRay.direction) / (i + 1);
            break;
        }

        mat3 primaryTangentspace;
        vec3 primaryNormalAvg = getNormalAndTangentSpaceForTriangle(primaryTriangle, primaryTangentspace);

        //check the light
        float primaryLight = getLight(primaryIntersectionPoint, primaryNormalAvg);

        addedColor += primaryLight / (i + 1);

        currentRay = makeRay(primaryIntersectionPoint, normalize(primaryTangentspace * rndVec));
    }

    fragColor = mix(oldFragColor, addedColor, 1.0/frameCounter);
    //fragColor = vec4(vec3(-1.0 + dist / 750.0), 0.5);
    //fragColor = vec4(vec3(float(primaryNearestIndex) * 0.01 + 0.01), 0.0);
    
    // }}}
    
    /* works
    // test rayTriangleIntersectionDistance {{{
    float dist = rayTriangleIntersectionDistance(
        currentRay, 
        vec3(-10.0, 10.0, 30.0), vec3(-10.0, -10.0, 20.0), vec3(10.0, 0.0, 27.0)) 
    );
    fragColor = vec4(vec3(-2.3 + 0.1 * dist, 0.5);
    // }}}
    */

    /* works
    // test rayBoxIntersectionDistance {{{
    float dist = rayBoxIntersectionDistance(
        currentRay, 
        vec3[2] (vec3(-3.0, -7.0, 20.0), vec3(5.0, -5.0, 30.0))
    );
    fragColor = vec4(vec3(-1.4 + 0.07 * dist), 0.5);
    // }}}
    */

    return;
}


// {{{ intersection tests

//compiles, tested
    // {{{ ray box intersection
float rayBoxIntersectionDistance(in Ray ray, in vec3 aabb[2]) {
    float tmin, tmax, tymin, tymax, tzmin, tzmax;
    tmin = (aabb[ray.sign[0]].x - ray.origin.x) * ray.inv_direction.x;
    tmax = (aabb[1-ray.sign[0]].x - ray.origin.x) * ray.inv_direction.x;
    tymin = (aabb[ray.sign[1]].y - ray.origin.y) * ray.inv_direction.y;
    tymax = (aabb[1-ray.sign[1]].y - ray.origin.y) * ray.inv_direction.y;
    if ( (tmin > tymax) || (tymin > tmax) ) return INFINITY;
    tmin = max(tmin, tymin);
    tmax = min(tmax, tymax);
    tzmin = (aabb[ray.sign[2]].z - ray.origin.z) * ray.inv_direction.z;
    tzmax = (aabb[1-ray.sign[2]].z - ray.origin.z) * ray.inv_direction.z;
    if ( (tmin > tzmax) || (tzmin > tmax) ) return INFINITY;
    tmin = max(tmin, tzmin);
    //tmax = min(tmay, tzmax); //not needed
    return tmin;
}
    // }}} ray box intersection

//compiles, tested
    // {{{ ray triangle intersection
float rayTriangleIntersectionDistance(
    in Ray ray,
    in vec3 v0, in vec3 v1, in vec3 v2)
{
    vec3 edge0 = v1 - v0;
    vec3 edge1 = v2 - v0;

    vec3 pvec = cross(ray.direction, edge1);

    float det = dot(edge0, pvec);

    //check parallel
    if (det > -EPSILON && det < EPSILON) return INFINITY;

    float inv_det = 1.0 / det;

    vec3 tvec = ray.origin - v0;

    float u = dot(tvec, pvec) * inv_det;
    if (u < 0.0 || u > 1.0)
        return INFINITY;

    vec3 qvec = cross(tvec, edge0);
    float v = dot(ray.direction, qvec) * inv_det;
    if (v < 0.0 || (u + v > 1.0))
        return INFINITY;

    return dot (edge1, qvec) * inv_det;
}
    // }}} ray triangle intersection 

    // {{{ ray bvh intersection
//compiles
//TODO: test
void rayBVHIntersection(
    in Ray ray,
    out int nearestIndex, out vec3 triangle[3], out vec3 intersectionPoint,
    out float dist)
{
    int numIndices;
    float distanceOfNearest;
    float distanceOfCurrent;
    int datapos;
    

    numIndices = textureSize(geometryBuffer);
    nearestIndex = -1;
    distanceOfNearest = INFINITY;
    distanceOfCurrent = INFINITY;
    datapos = 0;
    /*
    //TODO
    while (datapos < numIndices)
    {
        
        vec4 data_a = texelFetch(geometryBuffer, datapos);
        ++datapos;
        vec4 data_b = texelFetch(geometryBuffer, datapos);
        ++datapos;
        if (data_a.w == 0.0) // current node is an aabb
        {
            //TODO: overlapping boxes: we need a list of distanceOfCurrent variables
            //TODO!!!!!!!!!!: when ray is inside box?!?
            distanceOfCurrent = rayBoxIntersectionDistance(ray, vec3[2] (data_a.xyz, data_b.xyz) );
            //TODO: replace if with mix()
            if (!(distanceOfCurrent > EPSILON && distanceOfCurrent < distanceOfNearest)) {
                datapos = int(data_b.w);
                // TODO: data_b.w should be an integer. Problem: some integer operations are not officialy 
                // supported in OpenGL 3.2 core context (e.g. modulo, floatBitsToInt(f))
                // float32 has a 23bit mantissa
                //   ==> 2^23 = 8 388 608 (33.5 MB of float32-datafields addressable) 
                //   ==> currently we support about up to 2 ** 23 * (3/4 [[~1/4 aabbs]]) / 3 ~= 2 million triangles inside a bounding volume hierarchy
                //       (this number highly depends on the depth, resolution and breadth of the actual bvh)
            }
        }
        else //current node is a triangle
        {
            vec4 data_c = texelFetch(geometryBuffer, datapos);
            ++datapos;
            distanceOfCurrent = rayTriangleIntersectionDistance(
                ray,
                data_a.xyz, data_b.xyz, data_c.xyz);
            //if -> mix() ?
            if (distanceOfCurrent > EPSILON && distanceOfCurrent < distanceOfNearest) {
                nearestIndex = datapos - 3;
                distanceOfNearest = distanceOfCurrent;
            }
        }
    }
    */
    
    datapos = 2;
    while (datapos < numIndices) {//TODO: verify numIndices / replace with, let's say 30
        
        vec4 data_a = texelFetch(geometryBuffer, datapos);
        ++datapos;
        vec4 data_b = texelFetch(geometryBuffer, datapos);
        ++datapos;
        vec4 data_c = texelFetch(geometryBuffer, datapos);
        ++datapos;

        distanceOfCurrent = rayTriangleIntersectionDistance(
            ray,
            data_a.xyz, data_b.xyz, data_c.xyz);
        if (distanceOfCurrent > EPSILON && distanceOfCurrent < distanceOfNearest) {
            nearestIndex = datapos - 3;
            distanceOfNearest = distanceOfCurrent;
        }
    }
    
    triangle = vec3[3] (
        texelFetch(geometryBuffer, nearestIndex).xyz, 
        texelFetch(geometryBuffer, nearestIndex+1).xyz, 
        texelFetch(geometryBuffer, nearestIndex+2).xyz);
    //intersectionPoint = ray.origin + (distanceOfNearest - 0.001) * ray.direction;
    intersectionPoint = ray.origin -(ray.direction*0.001) + distanceOfNearest * ray.direction;

    dist = distanceOfNearest;
}

    // }}} ray bvh intersection

// }}} intersection tests


float getLight(vec3 pos, vec3 normal) {
    vec3 randomLightPos = (mix(arealight[2], arealight[3], rand2) + mix(arealight[0], arealight[1], rand)) / 2;

    bool lightShines = rayTriangleIntersectionBool(pos, randomLightPos);

    float cos = 0.0;
    if (lightShines) {
        cos = dot(normalize(randomLightPos - pos), normalize(normal))*0.6;
    }

    return cos;
}

//TODO: use as replacement for rayt...bool
//TODO: optimize (only test as far as needed)
/*bool canSee(vec3 eye, vec3 target) {
    vec3 direction = normalize(target - eye);

    int nearestIndex;
    vec3 triangle[3];
    vec3 intersectionPoint;

    rayBVHIntersection(makeRay(eye, direction), nearestIndex, triangle, intersectionPoint);

    return length(target-eye) < length(intersectionPoint-eye);
}*/

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