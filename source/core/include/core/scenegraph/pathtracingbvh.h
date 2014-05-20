#pragma once

#include <core/core_api.h>

#include <GL/glew.h>

#include <memory>
#include <vector>

#include <glm/vec4.hpp>


class Group;
class Node;

class CORE_API PathTracingBVH
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
