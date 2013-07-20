#include <qlist.h>
#include <glm/glm.hpp>

#include "group.h"
#include "polygonaldrawable.h"
#include "polygonalgeometry.h"
#include "core/aabb.h"
#include "core/bufferobject.h"
#include "pathtracingbvh.h"


PathTracingBVH::PathTracingBVH() : 
    m_geometry(new std::vector<glm::vec4>())
{

}

PathTracingBVH::~PathTracingBVH() {
    delete m_geometry;
}


void PathTracingBVH::buildTriangleList(Node *node) {
    m_geometry->clear();
    
    // depth first linearization //

    //add bounding box + meta data
    //        data1                data2
    // #### #### #### ####  #### #### #### ####
    // <--llf-------> isbox <--urb-------> *next
    /*improvement: data.w <= 0.0f ==> has sub-boxes / data.w > 0.0f ==> has data.w triangles --> less cond. branching and iteration overhead on gpu*/

    AxisAlignedBoundingBox aabb = node->boundingBox();
    m_geometry->push_back(glm::vec4(aabb.llf(), 0.0f)); //data1
    m_geometry->push_back(glm::vec4(aabb.urb(), 100000000000000000000000000.0f)); //data2 //there is no next aabb

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

//TODO: parameterize with function + merge into scenegraph
void PathTracingBVH::traverseNodeWithAdding(Node *node) {
    //add triangles
    for (auto child : node->children()) {
        //TODO: does this leaf test work?
        if (child->children().size() == 0) { //leaf = polygonal drawable
            // AAAAAAAAAAAAAAHHHHH god save me! This is so inconvenient and ugly! :§ :0 ...
            PolygonalDrawable *poly = dynamic_cast<PolygonalDrawable *>(child);
            QVector<glm::vec3> vertices = poly->geometry()->copyVertices();
            QVector<uint> indices = poly->geometry()->indices();
            for (int metaindex = 2; metaindex < indices.size(); ++metaindex) {
                //add triangle + meta data
                //        data1                data2                data3        
                // #### #### #### ####  #### #### #### ####  #### #### #### #### 
                // <--vertex 1--> istri <--vertex 1--> empty <--vertex 1--> empty
                /*improvement: global (not per-object) index, normal and vertex lists (but how?)*/
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
    //simple
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
