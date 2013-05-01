//#include <cstdlib>     
//#include <ctime>       

#include "vertexcacheoptimizer.h"
#include <vector>

const float FindVertexScore_CacheDecayPower = 1.5f;
const float FindVertexScore_LastTriScore = 0.75f;
const float FindVertexScore_ValenceBoostScale = 2.0f;
const float FindVertexScore_ValenceBoostPower = 0.5f;
const unsigned MaxSizeVertexCache = 32;
const float Scaler = 1.0f / (MaxSizeVertexCache - 3);


inline void swap(uint& a, uint& b)
{
    a ^= b ^= a ^= b;
}

struct Vertex;
struct Triangle;

struct Vertex {
    Vertex() :
        cachePosition(-1),
        numTriangles(0),
        numTrianglesToDo(0)
    {}

    int cachePosition;
    float score;
    int numTriangles;
    int numTrianglesToDo;
    std::vector<uint> triangles;
};

struct Triangle {
    Triangle() : 
        addedToDrawList(false)
    {}

    bool addedToDrawList;
    float score;
    std::vector<uint>vertices;
};

void initLists(t_uints &indices, std::vector<Vertex> &vertices, std::vector<Triangle> &triangles)
{
    for (auto it = indices.begin(); it != indices.end(); ++it) {
        vertices[*it].numTriangles++;
        vertices[*it].numTrianglesToDo++;
    }
    
    for (auto it = indices.begin(); it != indices.end(); ++it) {
        uint currentTriangleIndex = (it-indices.begin())/3;
        vertices[*it].triangles.push_back( currentTriangleIndex );
        triangles[currentTriangleIndex].vertices.push_back(*it);
    }

}

float findVertexScore(Vertex &vertex) {
    if (vertex.numTrianglesToDo == 0)
        return -1.0f;

    float score(0.0f);
    if (vertex.cachePosition >= 0)
    {
        if (vertex.cachePosition < 3)
            score = FindVertexScore_LastTriScore;
        else
        {
            score = 1.0f - (vertex.cachePosition - 3)*Scaler;
            score = powf(score, FindVertexScore_CacheDecayPower);
        }
    }

    float valenceBoost = powf(vertex.numTrianglesToDo, -FindVertexScore_ValenceBoostPower);

    score += FindVertexScore_ValenceBoostScale * valenceBoost;

    return score;
}


void VertexCacheOptimizer::applyOptimization(t_uints &indices, const int numVertices) {

    std::vector<Vertex> vertices(numVertices);
    std::vector<Triangle> triangles(indices.size()/3);

    initLists(indices, vertices, triangles);

    for (auto it = vertices.begin(); it != vertices.end(); ++it)
        it->score = findVertexScore(*it);

    int greatestTriangleIndex(0);
    for (auto it = triangles.begin(); it != triangles.end(); ++it)
    {
        it->score = vertices[it->vertices[0]].score
                  + vertices[it->vertices[1]].score
                  + vertices[it->vertices[2]].score;
        if (triangles[greatestTriangleIndex].score < it->score)
            greatestTriangleIndex = it-triangles.begin();
    }

    std::vector<int> cache(MaxSizeVertexCache+3, -1);
    indices.clear();
    indices.reserve(triangles.size()*3);

    while (greatestTriangleIndex != -1)
    {
        triangles[greatestTriangleIndex].addedToDrawList = true;
        // add greates Triangle to the draw list
        //dec trianglesToDo of its vertices and "sort" its triangle list
        for (int i = 0; i < 3; i++)
        {
            int currentVertexIndex = triangles[greatestTriangleIndex].vertices[i];
            Vertex& currentVertex = vertices[currentVertexIndex];
            indices.push_back(currentVertexIndex);

            //find the triangle in the vertices triangle list
            int t;
            for (t = 0; t < currentVertex.numTrianglesToDo; t++)
            {
                if (currentVertex.triangles[t] == greatestTriangleIndex)
                {
                    if (t == currentVertex.numTrianglesToDo-1)
                        break;
                    swap(currentVertex.triangles[t], currentVertex.triangles[currentVertex.numTrianglesToDo-1]);
                    break;
                }
            }
            currentVertex.numTrianglesToDo--;
        }


        // add/move the three vertices to the top of the LRU-Stack
        for (int vertexIndex = 0;  vertexIndex < 3; ++vertexIndex)
        {
            uint currentVertexIndex = triangles[greatestTriangleIndex].vertices[vertexIndex];
            int cachePosition = vertices[currentVertexIndex].cachePosition;
            //add a new element: and move all elements in the cache
            if (cachePosition == -1)
            {
                cachePosition = MaxSizeVertexCache+3-1;
            }
            for (int i = cachePosition; i > 0; i--)  
            {
                cache[i] = cache[i-1];
                if (cache[i] != -1)
                    vertices[cache[i]].cachePosition++;
            }
            cache[0] = currentVertexIndex;
            vertices[cache[0]].cachePosition = 0;
        }
        //update scores and remove overhang and find new greatest triangle
        greatestTriangleIndex = -1;
        float greatestTriangleScore = -1.0f;
        for (int v = 0; v < cache.size(); v++)
        {
            if (cache[v] == -1)
                break;
            Vertex& currentVertex = vertices[cache[v]];
            float oldScore = currentVertex.score;
            if(v >= MaxSizeVertexCache) 
            {
                currentVertex.cachePosition = -1;
                cache[v] = -1;
            }
            currentVertex.score = findVertexScore(currentVertex);
            for (int t = 0; t < currentVertex.numTrianglesToDo; t++) 
            {
                int currentTriangleIndex = currentVertex.triangles[t];
                triangles[currentTriangleIndex].score += currentVertex.score - oldScore;
                if (triangles[currentTriangleIndex].score > greatestTriangleScore)
                {
                    greatestTriangleIndex = currentTriangleIndex;
                    greatestTriangleScore = triangles[greatestTriangleIndex].score;
                }
            }
        }

        //if no greatestTriangle was found, iterate over all triangles
        for (int i = 0; i < triangles.size(); i++)
        {
            if (triangles[i].addedToDrawList)
                continue;
            if (triangles[i].score > greatestTriangleScore) 
            {
                greatestTriangleIndex = i;
                greatestTriangleScore = triangles[i].score;
            }
        }

    }
}