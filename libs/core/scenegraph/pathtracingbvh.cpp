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

void PathTracingBVH::buildBVHFromObjectsHierarchy(Node *node) {
    m_geometry->clear();
    AbsoluteTransformator abstrans = AbsoluteTransformator();
    Node *absolute = abstrans.toAbsoluteIgnoreRootTransform(node);
    addToGeometry(absolute);
}

void PathTracingBVH::addToGeometry(Node *node) {
    
    AxisAlignedBoundingBox aabb = node->boundingBox();
    int aabbIndex = m_geometry->size();
    m_geometry->push_back(glm::vec4()); //llf dummy
    m_geometry->push_back(glm::vec4()); //urb dummy

    //add triangles
    for (auto child : node->children()) {
        if (child->children().size() == 0) { //the current child is a leaf ==> it is a triangle object
            TriangleObject *triangleObject = dynamic_cast<TriangleObject *>(child);
            if (triangleObject == nullptr) { //we have to be safe because it could also be an empty Group
                qDebug() << "PathTracingBVH : addToGeometry(Node *) : dynamic cast to TriangleObject * failed";
                continue;
            }
            p_TriangleList triangles = triangleObject->triangles();
            for (int i = 2; i < triangles->size(); i += 3) {
                m_geometry->push_back(glm::vec4(triangles->at(i - 2), 3.0f)); // data1
                m_geometry->push_back(glm::vec4(triangles->at(i - 1), 0.0f)); // data2
                m_geometry->push_back(glm::vec4(triangles->at(i    ), 0.0f)); // data3
            }
        } else { //internal node
            addToGeometry(child);
        }
    }
    m_geometry->at(aabbIndex) = glm::vec4(aabb.llf(), 0.0f); //data1
    m_geometry->at(aabbIndex + 1) = glm::vec4(
        aabb.urb(), 
        float(m_geometry->size()) // points to one behind the last entry in geometry list
    ); //data2

}


std::vector<glm::vec4> *PathTracingBVH::geometry() {
    return m_geometry;
}
