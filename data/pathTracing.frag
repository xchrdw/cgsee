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

uniform int randomInt0;
uniform int randomInt1;
uniform int frameCounter;

vec3 light = vec3(0.0, 0.0, .0);
vec3 arealight[4] = vec3[]( vec3(343.0, 547.8, 227.0),
        vec3(343.0, 547.8, 282.0),
        vec3(213.0, 547.8, 332.0),
        vec3(213.0, 547.8, 227.0));

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

Ray makeRay(in vec3 origin, in vec3 direction) { //TODO: normalize direction?
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
    out int nearestIndex, out vec3 triangle[3], out vec3 intersectionPoint);
void rayBoxIntersectionDistances(
    in Ray ray, in vec3 aabb[2],
    out float tmin, out float tmax);
float rayTriangleIntersectionDistance(
    in Ray ray, 
    in vec3 v0, in vec3 v1, in vec3 v2);

void rayTriangleIntersection(Ray ray, out int nearestIndex, out vec3 triangle[3], out vec3 intersectionPoint);
bool rayTriangleIntersectionBool(vec3 origin, vec3 target);
vec3 getNormalAndTangentSpaceForTriangle(vec3 triangle[3], out mat3 tangentspace);
float getLight(vec3 pos, vec3 normal);
vec4 skybox(vec3 direction);


float rand =  fract(sin(dot(normalize(direction.xy) ,vec2(12.9898, 78.233)) * (randomInt0%1111)) * 43758.5453);
float rand2 =  fract(sin(dot(normalize(direction.xy) ,vec2(12.9898, 78.233)) * (randomInt1%1111)) * 43758.5453); // :)

//TODO: artifacts at the outside of the cornell box even for first order rays (maybe something like aabb-room-flickering))
//TODO: more artifacts for higher order rays

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

    
    for (int i = 0; i < 3; ++i) {
        rndVec = texelFetch(randomVectors, int(rand*numRnd)+i+1).xyz;
        //TODO: problems in both implementations: some strange rendering artifacts, 
        //different colors for the same object from different angles
        //works for 1 x aabb + triangles
        rayBVHIntersection(currentRay, primaryNearestIndex, primaryTriangle, primaryIntersectionPoint);
        //works:
        //rayTriangleIntersection(currentRay, primaryNearestIndex, primaryTriangle, primaryIntersectionPoint);

        if (primaryNearestIndex == -1) {
            addedColor += skybox(currentRay.direction) / (i + 1);
            break;
        }

        mat3 primaryTangentspace;
        vec3 primaryNormalAvg = getNormalAndTangentSpaceForTriangle(primaryTriangle, primaryTangentspace);

        //check the light
       // float primaryLight = getLight(primaryIntersectionPoint, primaryNormalAvg);

       // addedColor += primaryLight / (i + 1);

        //TODO: check if the ray can go through walls
        //addedColor = vec4(normalize(primaryTangentspace * rndVec), 1.0);
        //break;
        currentRay = makeRay(primaryIntersectionPoint, primaryTangentspace * rndVec);
    }

    fragColor = mix(oldFragColor, addedColor, 1.0/frameCounter);
    
    //fragColor = vec4(vec3(float(primaryNearestIndex) * 0.01 + 0.01), 0.0);
    
    // }}}
    
    /*
    // test rayTriangleIntersectionDistance {{{
    float dist = rayTriangleIntersectionDistance(
        currentRay, 
        vec3(-10.0, 10.0, 30.0), vec3(-10.0, -10.0, 20.0), vec3(10.0, 0.0, 27.0)) 
    );
    fragColor = vec4(vec3(-2.3 + 0.1 * dist, 0.5);
    // }}}
    */

    /*
    // test rayBoxIntersectionDistances {{{
    float tmin;
    float tmax;
    rayBoxIntersectionDistances(
        makeRay(
            vec3(-2.0, 2.0, -3.0), 
            normalize(vec3((v_uv.x - 0.5 + 0.5) * 2.0, (v_uv.y - 0.5 - 0.5) * 2.0, 1.0))
        ), 
        vec3[2] (
            vec3(-1.0, -1.0, -1.0), 
            vec3(1.0, 1.0, 1.0)
        ),
        tmin, tmax
    );
    if (tmin < tmax) {
        fragColor = vec4(0.0, tmin * 0.15, tmax * 0.15, 0.5);
    } else {
        fragColor = vec4(1.0, 0.0, 0.0, 0.5);
    }
    // }}}
    */

    return;
}


// {{{ intersection tests

//compiles, tested
    // {{{ ray box intersection
void rayBoxIntersectionDistances(
    in Ray ray, in vec3 aabb[2],
    out float tmin, out float tmax)
{
    float tymin, tymax, tzmin, tzmax;
    tmin = (aabb[ray.sign[0]].x - ray.origin.x) * ray.inv_direction.x;
    tmax = (aabb[1-ray.sign[0]].x - ray.origin.x) * ray.inv_direction.x;
    tymin = (aabb[ray.sign[1]].y - ray.origin.y) * ray.inv_direction.y;
    tymax = (aabb[1-ray.sign[1]].y - ray.origin.y) * ray.inv_direction.y;
    tzmin = (aabb[ray.sign[2]].z - ray.origin.z) * ray.inv_direction.z;
    tzmax = (aabb[1-ray.sign[2]].z - ray.origin.z) * ray.inv_direction.z;
    tmin = max(max(tmin, tymin), tzmin);
    tmax = min(min(tmax, tymax), tzmax);
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
    if (det > -EPSILON && det < EPSILON)
        return INFINITY;

    float inv_det = 1.0 / det;

    vec3 tvec = ray.origin - v0;

    float u = dot(tvec, pvec) * inv_det;
    vec3 qvec = cross(tvec, edge0);
    float v = dot(ray.direction, qvec) * inv_det;
    if (v < 0.0 || u < 0.0 || (u + v > 1.0))
        return INFINITY;

    return dot (edge1, qvec) * inv_det;
}
    // }}} ray triangle intersection 

    // {{{ ray bvh intersection
//compiles
//TODO: test
void rayBVHIntersection(
    in Ray ray,
    out int nearestIndex, out vec3 triangle[3], out vec3 intersectionPoint)
{
    int numIndices;
    float distanceOfNearest;
    //float distanceOfCurrent;
    float tmin;
    float tmax;
    int datapos;
    

    numIndices = textureSize(geometryBuffer);
    nearestIndex = -1;
    distanceOfNearest = INFINITY;
    //distanceOfCurrent = INFINITY;
    datapos = 0;
    
    //TODO
    
    while (datapos < numIndices)
    {
        
        vec4 data_a = texelFetch(geometryBuffer, datapos);
        ++datapos;
        vec4 data_b = texelFetch(geometryBuffer, datapos);
        ++datapos;
        //algorithm only works for non-intersection bounding boxes!
        if (data_a.w == 0.0) // current node is an aabb
        {
            //TODO: overlapping boxes: we need a list of tmin variables
            //TODO!!!!!!!!!!: if ray is inside box?!?
            rayBoxIntersectionDistances(ray, vec3[2] (data_a.xyz, data_b.xyz), tmin, tmax );
            //TODO: replace if with mix()
            if (!(      tmin < tmax //ray intersects bounding box
                    &&  tmax > EPSILON //intersection point with backside of bounding box is in the direction the ray points to (we use the backside to handle rays inside the bounding box correctly)
                    )) {
                datapos = int(data_b.w);
                // TODO: data_b.w should be an integer. Problem: some integer operations are not officially 
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
            tmin = rayTriangleIntersectionDistance(
                ray,
                data_a.xyz, data_b.xyz, data_c.xyz);
            //if -> mix() ?
            if (    tmin > EPSILON //intersection point lies in the direction the ray points
                &&  tmin < distanceOfNearest //intersection point lies before the triangles tested yet
                ) {
                nearestIndex = datapos - 3;
                distanceOfNearest = tmin;
            }
        }
    }

    triangle = vec3[3] (
        texelFetch(geometryBuffer, nearestIndex).xyz, 
        texelFetch(geometryBuffer, nearestIndex+1).xyz, 
        texelFetch(geometryBuffer, nearestIndex+2).xyz);
    //intersectionPoint = ray.origin + (distanceOfNearest - 0.001) * ray.direction;
    intersectionPoint = ray.origin + distanceOfNearest * ray.direction * 0.999;
}

    // }}} ray bvh intersection

// }}} intersection tests


float getLight(vec3 pos, vec3 normal) {
    vec3 randomLightPos = arealight[0] + ((arealight[1] - arealight[0]) * rand) + ((arealight[3] - arealight[0]) * rand2);

    bool lightShines = rayTriangleIntersectionBool(pos, randomLightPos);

    float cos = 0.0;
    if (lightShines) {
        cos = dot(normalize(randomLightPos - pos), normalize(normal))*0.6;
    }

    return cos;
}

//TODO: use as replacement for rayt...bool
//TODO: optimize (only test as far as needed)
bool canSee(vec3 eye, vec3 target) {
    vec3 direction = normalize(target - eye);

    int nearestIndex;
    vec3 triangle[3];
    vec3 intersectionPoint;

    rayBVHIntersection(makeRay(eye, direction), nearestIndex, triangle, intersectionPoint);

    return length(target-eye) < length(intersectionPoint-eye);
}

bool rayTriangleIntersectionBool(vec3 origin, vec3 target) {
    vec3 originToTarget = target - origin;

    int nearestIndex;
    vec3 triangle[3];
    vec3 intersectionPoint;

    rayTriangleIntersection(makeRay(origin, originToTarget), nearestIndex, triangle, intersectionPoint);

    return length(originToTarget) < length(intersectionPoint-origin);
}


void rayTriangleIntersection(Ray ray, out int nearestIndex, out vec3 triangle[3], out vec3 intersectionPoint) {
    int numIndices = textureSize(indexBuffer);
    nearestIndex = -1;
    float distanceOfNearest = 100000.0;

    for (int i = 0; i < numIndices - 2; i += 3) {
        vec3 v0 = texelFetch(vertexBuffer, int(texelFetch(indexBuffer, i + 0).x)).xyz;
        vec3 v1 = texelFetch(vertexBuffer, int(texelFetch(indexBuffer, i + 1).x)).xyz;
        vec3 v2 = texelFetch(vertexBuffer, int(texelFetch(indexBuffer, i + 2).x)).xyz;

        float t = rayTriangleIntersectionDistance(ray,  v0, v1, v2);

        if (t < distanceOfNearest && t > EPSILON) {
            distanceOfNearest = t;
            nearestIndex = i;
            triangle[0] = v0;
            triangle[1] = v1;
            triangle[2] = v2;
        }
    }

    intersectionPoint = ray.origin + distanceOfNearest * ray.direction * 0.999;
}

vec4 skybox(vec3 direction) {
    // return vec4(0.09, 0.6, 0.9, 1.0);
    return vec4(1.0, 1.0, 1.0, 1.0);
}

vec3 getNormalAndTangentSpaceForTriangle(vec3 triangle[3], out mat3 tangentspace) {
    vec3 e0 = triangle[1] - triangle[0];
    vec3 e1 = triangle[2] - triangle[0];

    vec3 norm_normal = normalize(cross(e0, e1));
    vec3 anotherone = normalize(vec3(rand+0.5, rand, rand));


    tangentspace[0] = normalize(cross(norm_normal, anotherone));        // t
    tangentspace[1] = norm_normal;          // n
    tangentspace[2] = cross(tangentspace[1], tangentspace[0]);  // n X t
    
    //return vec3(0.0, 1.0, 0.0);
    return norm_normal;
}