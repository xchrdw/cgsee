#include <qdebug.h>

#include <qlist.h>
#include <glm/glm.hpp>

#include "group.h"
#include "polygonaldrawable.h"
#include "polygonalgeometry.h"
#include "core/aabb.h"
#include "core/bufferobject.h"
#include "pathtracingbvh.h"

//TODO: pass float.infinity to shader

PathTracingBVH::PathTracingBVH() : 
    m_geometry(new std::vector<glm::vec4>())
{

}

PathTracingBVH::~PathTracingBVH() {
    delete m_geometry;
}

//TODO: (pathtracer.cpp) empty bvh

void PathTracingBVH::buildTriangleList(Node *node) {
    m_geometry->clear();
    
    // depth first linearization //

    //add bounding box + meta data
    //        data1                data2
    // #### #### #### ####  #### #### #### ####
    // <--llf-------> isbox <--urb-------> *next
    /*improvement: data.w <= 0.0f ==> has sub-boxes / data.w > 0.0f ==> has data.w triangles --> less cond. branching and iteration overhead on gpu*/

    glm::vec3 antiAABBFlickeringEpsilon = glm::vec3(0.00001f, 0.00001f, 0.00001f);

    AxisAlignedBoundingBox aabb = node->boundingBox();
    m_geometry->push_back(glm::vec4(aabb.llf() - antiAABBFlickeringEpsilon, 0.0f)); //data1
    m_geometry->push_back(glm::vec4(aabb.urb() + antiAABBFlickeringEpsilon, 10000000.0f)); //data2 //there is no next aabb //TODO: INT_MAX or even better: bytes of int to float and backwards on gpu (support bigger models)
    
    //add triangles
    //TODO: use iterator / traverser of scenegraph + use settings of gui (e.g. culling)
    traverseNodeWithAdding(node);

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

//TODO: can't deal with empty vertex / index lists (leaf child can't be dynamically casted to PolygonalDrawable), see #*#*#

//TODO: parameterize with function + merge into scenegraph
void PathTracingBVH::traverseNodeWithAdding(Node *node) {
    //add triangles
    for (auto child : node->children()) {
        //TODO: does this leaf test always work?
        if (child->children().size() == 0) { //leaf = polygonal drawable
            // AAAAAAAAAAAAAAHHHHH god save me! This is so inconvenient and ugly! :§ :0 ...
            PolygonalDrawable *poly = dynamic_cast<PolygonalDrawable *>(child); // #*#*#: poly becomes nullptr if we load the "null-model" with zero triangles
            QVector<glm::vec3> vertices = poly->geometry()->copyVertices();
            QVector<uint> indices = poly->geometry()->indices();
            for (int metaindex = 2; metaindex < indices.size(); metaindex += 3) {
                //add triangle + meta data
                //        data1                data2                data3        
                // #### #### #### ####  #### #### #### ####  #### #### #### #### 
                // <--vertex 1--> istri <--vertex 2--> empty <--vertex 3--> empty
                //(maybe an) improvement: global (not per-object) index, normal and vertex lists (but how to do that?)
                m_geometry->push_back(glm::vec4(vertices[indices[metaindex-2]], 3.0f)); // data1
                m_geometry->push_back(glm::vec4(vertices[indices[metaindex-1]], 0.0f)); // data2
                m_geometry->push_back(glm::vec4(vertices[indices[metaindex]],   0.0f)); // data3
            }
        } else { //internal node
            traverseNodeWithAdding(child);
        }
    }
}



void PathTracingBVH::buildBVHFromObjectsHierarchy(Node *node) {
    m_geometry->clear();
    
    // depth first linearization //

    //add triangles
    //TODO: use iterator / traverser of scenegraph + use settings of gui (e.g. culling)
    traverseNodeWithAddingAndSubNodes(node);

}

//TODO: parameterize with function + merge into scenegraph
void PathTracingBVH::traverseNodeWithAddingAndSubNodes(Node *node) {
    glm::vec3 antiAABBFlickeringEpsilon = glm::vec3(0.00001f, 0.00001f, 0.00001f);

    AxisAlignedBoundingBox aabb = node->boundingBox();
    int aabbIndex = m_geometry->size();
    m_geometry->push_back(glm::vec4(0.0f)); //data1
    m_geometry->push_back(glm::vec4(0.0f)); //data2

    //add triangles
    for (auto child : node->children()) {
        //TODO: does this leaf test work?
        if (child->children().size() == 0) { //leaf = polygonal drawable
            PolygonalDrawable *poly = dynamic_cast<PolygonalDrawable *>(child); // poly becomes nullptr if we load the "null-model" with zero triangles
            QVector<glm::vec3> vertices = poly->geometry()->copyVertices();
            QVector<uint> indices = poly->geometry()->indices();
            for (int metaindex = 2; metaindex < indices.size(); metaindex += 3) {
                //add triangle + meta data
                //        data1                data2                data3        
                // #### #### #### ####  #### #### #### ####  #### #### #### #### 
                // <--vertex 1--> istri <--vertex 2--> empty <--vertex 3--> empty
                //(maybe an) improvement: global (not per-object) index, normal and vertex lists (but how to do that?)
                m_geometry->push_back(glm::vec4(vertices[indices[metaindex-2]], 3.0f)); // data1
                m_geometry->push_back(glm::vec4(vertices[indices[metaindex-1]], 0.0f)); // data2
                m_geometry->push_back(glm::vec4(vertices[indices[metaindex]],   0.0f)); // data3
            }
        } else { //internal node
            traverseNodeWithAddingAndSubNodes(child);
        }
    }
    //add bounding box + meta data
    //        data1                data2
    // #### #### #### ####  #### #### #### ####
    // <--llf-------> isbox <--urb-------> *next
    /*improvement: data.w <= 0.0f ==> has sub-boxes / data.w > 0.0f ==> has data.w triangles --> less cond. branching and iteration overhead on gpu*/

    m_geometry->at(aabbIndex) = glm::vec4(aabb.llf() - antiAABBFlickeringEpsilon, 0.0f); //data1
    m_geometry->at(aabbIndex + 1) = glm::vec4(aabb.urb() + antiAABBFlickeringEpsilon, float(m_geometry->size())); //data2

}

void PathTracingBVH::buildBVHFromObjectsHierarchyWithSplitting(Node *node) {
    //TODO
}

void PathTracingBVH::buildBVHOnlyWithSplitting(Node *node) {
    //TODO
}

std::vector<glm::vec4> *PathTracingBVH::geometry() {
    return m_geometry;
}
