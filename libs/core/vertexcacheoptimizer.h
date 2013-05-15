#pragma once

#include "common.h"

class VertexCacheOptimizer
{
public:
	static void applyOptimization(t_uints &stuff, const int numVertices);

protected:
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

    static void initLists(t_uints &indices, std::vector<Vertex> &vertices, std::vector<Triangle> &triangles);
    static void initScores(std::vector<Vertex> &vertices, std::vector<Triangle> &triangles);
    static void addGreatestTriangle(t_uints &indices, std::vector<Vertex> &vertices, std::vector<Triangle> &triangles, int &greatestTriangleIndex);
    static void pushVerticesToStack(std::vector<Vertex> &vertices, std::vector<Triangle> &triangles, int &greatestTriangleIndex, std::vector<int> &cache);
    static void updateCacheAndFindGreatestTriangle(std::vector<Vertex> &vertices, std::vector<Triangle> &triangles, int &greatestTriangleIndex, std::vector<int> &cache);
    static int findGreatestTriangle(std::vector<Triangle> &triangles);
    static float calculateVertexScore(Vertex &vertex);
};

