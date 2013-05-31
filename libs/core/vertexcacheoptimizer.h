#pragma once

#include "common.h"

class VertexCacheOptimizer
{
public:
	static void applyOptimization(t_uints &indices, const uint numVertices);

protected:
    struct Vertex {
        Vertex() :
            cachePosition(-1),
            numTriangles(0),
            numTrianglesToDo(0)
        {}

        int cachePosition;
        float score;
        uint numTriangles;
        uint numTrianglesToDo;
        std::vector<int> triangles;
    };

    struct Triangle {
        Triangle() : 
            addedToDrawList(false)
        {}

        bool addedToDrawList;
        float score;
        std::vector<uint>vertices;
    };

    static void initLists(const t_uints &indices, std::vector<Vertex> &vertices, std::vector<Triangle> &triangles);
    static void initScores(std::vector<Vertex> &vertices, std::vector<Triangle> &triangles);
    static float calculateVertexScore(const Vertex &vertex);
    static void addGreatestTriangle(t_uints &indices, std::vector<Vertex> &vertices, std::vector<Triangle> &triangles, int &greatestTriangleIndex);
    static void pushVerticesToStack(std::vector<Vertex> &vertices, const std::vector<Triangle> &triangles, const int greatestTriangleIndex, std::vector<int> &cache);
    static void updateCacheAndFindGreatestTriangle(std::vector<Vertex> &vertices, std::vector<Triangle> &triangles, int &greatestTriangleIndex, std::vector<int> &cache);
    static int findGreatestTriangle(const std::vector<Triangle> &triangles);
};

