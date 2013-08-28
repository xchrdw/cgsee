#pragma once

#include <memory>
#include <vector>

#include <core/declspec.h>

class Group;

class CGSEE_API PathTracingBVH
{
public:
    PathTracingBVH();
    ~PathTracingBVH();

    void buildFlatBVH(Node *node);
    void buildBVHFromObjectsHierarchy(Node *node);

    void traverseNodeWithAdding(Node *node);
    void traverseNodeWithAddingAndSubNodes(Node *node);

    void geometryToTexture(GLuint textureSlot);

    std::vector<glm::vec4> *geometry();

protected:
    std::vector<glm::vec4> *m_geometry;
};