#include <qdebug.h>

#include <qlist.h>
#include <glm/glm.hpp>

#include "group.h"
#include "core/aabb.h"
#include "core/bufferobject.h"
#include "pathtracingbvh.h"
#include "absolutetransformator.h"
#include "triangleobject.h"

//TODO: pass float.infinity to shader

PathTracingBVH::PathTracingBVH()
:   m_geometry(new std::vector<glm::vec4>())
{

}

PathTracingBVH::~PathTracingBVH() {
    delete m_geometry;
}

void PathTracingBVH::buildFlatBVH(Node *node) {
    m_geometry->clear();
    
    // depth first linearization //

    glm::vec3 antiAABBFlickeringEpsilon = glm::vec3(0.00001f, 0.00001f, 0.00001f);
    
    AbsoluteTransformator abstrans = AbsoluteTransformator();
    Node *absolute = abstrans.toAbsoluteIgnoreRootTransform(node);

    AxisAlignedBoundingBox aabb = absolute->boundingBox();
    m_geometry->push_back(glm::vec4(aabb.llf() - antiAABBFlickeringEpsilon, 0.0f)); //data1
    m_geometry->push_back(glm::vec4(aabb.urb() + antiAABBFlickeringEpsilon, 10000000.0f)); //data2 //there is no next aabb //TODO: INT_MAX or even better: bytes of int to float and backwards on gpu (support bigger models)
    
    //add triangles
    //TODO: use iterator / traverser of scenegraph + use settings of gui (e.g. culling)
    traverseNodeWithAdding(absolute);

    
}

void PathTracingBVH::geometryToTexture(GLuint textureSlot) {
    BufferObject *geometryBuffer = new BufferObject(GL_TEXTURE_BUFFER, GL_STATIC_READ);
    geometryBuffer->data<glm::vec4>(m_geometry->data(), m_geometry->size(), GL_RGBA32F, sizeof(glm::vec4));
    glActiveTexture(textureSlot);
    GLuint textureHandle;
    glGenTextures(1, &textureHandle);
    glBindTexture(GL_TEXTURE_BUFFER, textureHandle);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, geometryBuffer->buffer());
    glError();
}

void PathTracingBVH::traverseNodeWithAdding(Node *node) {
    for (auto child : node->children()) {
        // TODO : This is not a nice leaf test. Should we add a function isLeaf() to the Node class?
        if (child->children().size() == 0) { //the current child is a leaf ==> it is a triangle object
            TriangleObject *triangleObject = dynamic_cast<TriangleObject *>(child);
            if (triangleObject == nullptr) { //we have to be safe because it could also be an empty Group
                qDebug() << "PathTracingBVH : traverseNodeWithAdding(Node *) : dynamic cast to TriangleObject * failed";
                continue;
            }
            p_TriangleList triangles = triangleObject->triangles();
            for (int i = 2; i < triangles->size(); i += 3) {
                m_geometry->push_back(glm::vec4(triangles->at(i - 2), 3.0f)); // data1
                m_geometry->push_back(glm::vec4(triangles->at(i - 1), 0.0f)); // data2
                m_geometry->push_back(glm::vec4(triangles->at(i    ), 0.0f)); // data3
            }
        } else { //internal node
            traverseNodeWithAdding(child);
        }
    }
}



void PathTracingBVH::buildBVHFromObjectsHierarchy(Node *node) {
    m_geometry->clear();
    Node *absoluteNode = node;
    traverseNodeWithAddingAndSubNodes(node);
}

void PathTracingBVH::traverseNodeWithAddingAndSubNodes(Node *node) {
    
    AbsoluteTransformator abstrans = AbsoluteTransformator();
    Node *absolute = abstrans.toAbsoluteIgnoreRootTransform(node);

    glm::vec4 dbg;
    AxisAlignedBoundingBox aabb = absolute->boundingBox();
    /*dbg = glm::vec4(aabb.llf(), 0.0f);
    //qDebug() << "aabb  " << dbg.x << " " << dbg.y << " " << dbg.z << " " << dbg.w;
    dbg = glm::vec4(aabb.urb(), 0.0f);
    //qDebug() << "aabb  " << dbg.x << " " << dbg.y << " " << dbg.z << " " << dbg.w;*/
    int aabbIndex = m_geometry->size();
    m_geometry->push_back(glm::vec4()); //llf dummy
    m_geometry->push_back(glm::vec4()); //urb dummy

    //add triangles
    for (auto child : node->children()) {
        // TODO : This is not a nice leaf test. Should we add a function isLeaf() to the Node class?
        if (child->children().size() == 0) { //the current child is a leaf ==> it is a triangle object
            TriangleObject *triangleObject = dynamic_cast<TriangleObject *>(child);
            if (triangleObject == nullptr) { //we have to be safe because it could also be an empty Group
                qDebug() << "PathTracingBVH : traverseNodeWithAddingAndSubNodes(Node *) : dynamic cast to TriangleObject * failed";
                continue;
            }
            p_TriangleList triangles = triangleObject->triangles();
            for (int i = 2; i < triangles->size(); i += 3) {
                m_geometry->push_back(glm::vec4(triangles->at(i - 2), 3.0f)); // data1
                m_geometry->push_back(glm::vec4(triangles->at(i - 1), 0.0f)); // data2
                m_geometry->push_back(glm::vec4(triangles->at(i    ), 0.0f)); // data3
            }
        } else { //internal node
            traverseNodeWithAddingAndSubNodes(child);
        }
    }
    //add bounding box + meta data
    //        data1                data2
    // #### #### #### ####  #### #### #### ####
    // <--llf-------> isbox <--urb-------> *next
    /*improvement: data.w <= 0.0f ==> has sub-boxes / data.w > 0.0f ==> has data.w triangles --> less cond. branching and iteration overhead on gpu, one more datafield for each triangle*/

    m_geometry->at(aabbIndex) = glm::vec4(aabb.llf(), 0.0f); //data1
    m_geometry->at(aabbIndex + 1) = glm::vec4(
        aabb.urb(), 
        float(m_geometry->size()) // points to one behind the last entry in geometry list
    ); //data2

}


std::vector<glm::vec4> *PathTracingBVH::geometry() {
    return m_geometry;
}
