
#include <core/scenegraph/pathtracingbvh.h>

#include <qdebug.h>

#include <qlist.h>
#include <glm/glm.hpp>

#include <core/scenegraph/group.h>
#include <core/aabb.h>
#include <core/bufferobject.h>
#include <core/scenegraph/absolutetransformator.h>
#include <core/scenegraph/triangleobject.h>

//TODO: pass float.infinity to shader

PathTracingBVH::PathTracingBVH()
:   m_geometry(new std::vector<glm::vec4>())
{
}

PathTracingBVH::~PathTracingBVH() {
    delete m_geometry;
}

void PathTracingBVH::geometryToTexture(gl::GLuint textureSlot) {
    BufferObject geometryBuffer(gl::GLenum::GL_TEXTURE_BUFFER, gl::GLenum::GL_STATIC_READ);
    geometryBuffer.data<glm::vec4>(m_geometry->data(), m_geometry->size(), gl::GLenum::GL_RGBA32F, sizeof(glm::vec4));
    gl::glActiveTexture(textureSlot);
    gl::GLuint textureHandle;
    gl::glGenTextures(1, &textureHandle);
    gl::glBindTexture(gl::GLenum::GL_TEXTURE_BUFFER, textureHandle);
    gl::glTexBuffer(gl::GLenum::GL_TEXTURE_BUFFER, gl::GLenum::GL_RGBA32F, geometryBuffer.buffer());

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
    glm::vec3 aabbEnlargement = aabb.radius() * glm::vec3(0.000001f, 0.000001f, 0.000001f);
    m_geometry->at(aabbIndex) = glm::vec4(
        aabb.llf() - aabbEnlargement, //llf
        0.0f //we are a bounding box
    ); //data1
    m_geometry->at(aabbIndex + 1) = glm::vec4(
        aabb.urb() + aabbEnlargement, //urb 
        float(m_geometry->size()) // points to one behind the last entry in geometry list, 
        // that is the next object on the same hierarchy level (or a higher hierarchy level 
        // if there is none on the same)
    ); //data2

}


std::vector<glm::vec4> *PathTracingBVH::geometry() {
    return m_geometry;
}
