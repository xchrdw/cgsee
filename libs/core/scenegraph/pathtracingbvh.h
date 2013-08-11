#pragma once

#include <memory>
#include <vector>

#include <core/declspec.h>

class Group;
class DataBlockRegistry;

class CGSEE_API PathTracingBVH
{
public:
    PathTracingBVH(std::shared_ptr<DataBlockRegistry> registry);
    ~PathTracingBVH();

    void buildFlatBVH(Node *node);
    void buildBVHFromObjectsHierarchy(Node *node);

    void traverseNodeWithAdding(Node *node);
    void traverseNodeWithAddingAndSubNodes(Node *node);

    void geometryToTexture(GLuint textureSlot);

    std::vector<glm::vec4> *geometry();

protected:
    std::vector<glm::vec4> *m_geometry;
    std::shared_ptr<DataBlockRegistry> m_registry;
};

/*
TODOs / ideas:
==============
- make changing geometry possible (add / remove)
- handling of vertex normals, attributes, ...
*/
