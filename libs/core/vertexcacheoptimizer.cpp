#include <vector>
#include "vertexcacheoptimizer.h"

namespace 
{
    const float CACHE_DECAY_POWER = 1.5f;
    const float LAST_TRI_SCORE = 0.75f;
    const float VALENCE_BOOST_SCALE = 2.0f;
    const float VALENCE_BOOST_POWER = 0.5f;
    const unsigned MAX_SIZE_VERTEX_CACHE = 32;
    const float SCALER = 1.0f / (MAX_SIZE_VERTEX_CACHE - 3);
}


inline void swap(uint& a, uint& b)
{
    a ^= b ^= a ^= b;
}

void VertexCacheOptimizer::initLists(t_uints &indices, std::vector<Vertex> &vertices, std::vector<Triangle> &triangles)
{
    for (uint index : indices) {
        ++vertices[index].numTriangles;
        ++vertices[index].numTrianglesToDo;
    }
    
    for (auto it = indices.cbegin(); it != indices.cend(); ++it) {
        uint currentTriangleIndex = (it - indices.cbegin())/3;
        vertices[*it].triangles.push_back( currentTriangleIndex );
        triangles[currentTriangleIndex].vertices.push_back(*it);
    }

}

void VertexCacheOptimizer::initScores(std::vector<Vertex> &vertices, std::vector<Triangle> &triangles)
{
    for (Vertex &vertex : vertices) 
        vertex.score = calculateVertexScore(vertex);
    
    for (Triangle &triangle : triangles) {
        triangle.score = vertices[triangle.vertices[0]].score
                       + vertices[triangle.vertices[1]].score
                       + vertices[triangle.vertices[2]].score;
    }
}

float VertexCacheOptimizer::calculateVertexScore(Vertex &vertex) {
    if (vertex.numTrianglesToDo == 0)
        return -1.0f;

    float score(0.0f);
    if (vertex.cachePosition >= 0) {
        if (vertex.cachePosition < 3)
            score = LAST_TRI_SCORE;
        else {
            score = 1.0f - (vertex.cachePosition - 3) * SCALER;
            score = powf(score, CACHE_DECAY_POWER);
        }
    }

    float valenceBoost = powf(vertex.numTrianglesToDo, -VALENCE_BOOST_POWER);

    score += VALENCE_BOOST_SCALE * valenceBoost;

    return score;
}

void VertexCacheOptimizer::addGreatestTriangle(t_uints &indices, std::vector<Vertex> &vertices, std::vector<Triangle> &triangles, int &greatestTriangleIndex)
{
    triangles[greatestTriangleIndex].addedToDrawList = true;
    // add greates Triangle to the draw list
    // dec trianglesToDo of its vertices and "sort" its triangle list
    for (int i = 0; i < 3; ++i) {
        int currentVertexIndex = triangles[greatestTriangleIndex].vertices[i];
        Vertex& currentVertex = vertices[currentVertexIndex];
        indices.push_back(currentVertexIndex);

        //find the triangle in the vertices triangle list
        for (int t = 0; t < currentVertex.numTrianglesToDo; ++t) {
            if (currentVertex.triangles[t] == greatestTriangleIndex) {
                if (t == currentVertex.numTrianglesToDo - 1)
                    break;
                swap(currentVertex.triangles[t], currentVertex.triangles[currentVertex.numTrianglesToDo - 1]);
                break;
            }
        }
        --currentVertex.numTrianglesToDo;
    }
}

void VertexCacheOptimizer::pushVerticesToStack(std::vector<Vertex> &vertices, std::vector<Triangle> &triangles, int &greatestTriangleIndex, std::vector<int> &cache)
{
    // add/move the three vertices to the top of the LRU-Stack
    for (int vertexIndex = 0;  vertexIndex < 3; ++vertexIndex) {
        uint currentVertexIndex = triangles[greatestTriangleIndex].vertices[vertexIndex];
        int cachePosition = vertices[currentVertexIndex].cachePosition;
        //if we add a new element to the cache we need to move all elements in it
        if (cachePosition == -1) {
            cachePosition = MAX_SIZE_VERTEX_CACHE + 3 - 1;
        }
        //move all elements from index 1 to cachePosition
        for (int i = cachePosition; i > 0; --i)   {
            cache[i] = cache[i - 1];
            if (cache[i] != -1)
                ++vertices[cache[i]].cachePosition;
        }
        //move the new element to the top of the cache
        cache[0] = currentVertexIndex;
        vertices[cache[0]].cachePosition = 0;
    }
}

//update scores, remove overhang and find new greatest triangle
void VertexCacheOptimizer::updateCacheAndFindGreatestTriangle(std::vector<Vertex> &vertices, std::vector<Triangle> &triangles, 
                                                              int &greatestTriangleIndex, std::vector<int> &cache)
{
    greatestTriangleIndex = -1;
    float greatestTriangleScore = -1.0f;
    for (int v = 0; v < cache.size(); ++v) {
        if (cache[v] == -1)
            break;
        Vertex& currentVertex = vertices[cache[v]];
        float oldScore = currentVertex.score;
        //remove overhang
        if(v >= MAX_SIZE_VERTEX_CACHE) {
            currentVertex.cachePosition = -1;
            cache[v] = -1;
        }
        currentVertex.score = calculateVertexScore(currentVertex);
        //update the scores of this vertices triangles
        for (int t = 0; t < currentVertex.numTrianglesToDo; ++t)  {
            int currentTriangleIndex = currentVertex.triangles[t];
            triangles[currentTriangleIndex].score += currentVertex.score - oldScore;
            if (triangles[currentTriangleIndex].score > greatestTriangleScore) {
                greatestTriangleIndex = currentTriangleIndex;
                greatestTriangleScore = triangles[greatestTriangleIndex].score;
            }
        }
    }
}

//scan the complete triangle list for the best triangle
int VertexCacheOptimizer::findGreatestTriangle(std::vector<Triangle> &triangles)
{
    float greatestTriangleScore(-1.0);
    int greatestTriangleIndex(-1);
    for (auto it = triangles.cbegin(); it != triangles.cend(); ++it) {
        if (it->addedToDrawList)
            continue;
        if (it->score > greatestTriangleScore) {
            greatestTriangleIndex = it-triangles.cbegin();
            greatestTriangleScore = it->score;
        }
    }
    return greatestTriangleIndex;
}

void VertexCacheOptimizer::applyOptimization(t_uints &indices, const int numVertices)
{
    std::vector<Vertex> vertices(numVertices);
    std::vector<Triangle> triangles(indices.size() / 3);
    std::vector<int> cache(MAX_SIZE_VERTEX_CACHE + 3, -1);

    initLists(indices, vertices, triangles);

    initScores(vertices, triangles);

    int greatestTriangleIndex = findGreatestTriangle(triangles);

    indices.clear();
    indices.reserve(triangles.size() * 3);

    while (greatestTriangleIndex != -1) {
        addGreatestTriangle(indices, vertices, triangles, greatestTriangleIndex);

        pushVerticesToStack(vertices, triangles, greatestTriangleIndex, cache);

        updateCacheAndFindGreatestTriangle(vertices, triangles, greatestTriangleIndex, cache);

        //if no greatestTriangle was found, iterate over all triangles
        if (greatestTriangleIndex == -1)
            greatestTriangleIndex = findGreatestTriangle(triangles);
    }
}
