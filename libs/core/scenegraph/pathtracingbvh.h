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
    
    void geometryToTexture(GLuint textureSlot);

    void buildBVHFromObjectsHierarchy(Node *node);
    void addToGeometry(Node *node);

    //TODO: implement for deeper bounding volume hierarchies
    //void buildBVHFromObjectsHierarchyAndSplitObjects(Node *node);
    //void addToGeometrySplitObjects(Node *node);

    std::vector<glm::vec4> *geometry();

protected:
    std::vector<glm::vec4> *m_geometry;
};