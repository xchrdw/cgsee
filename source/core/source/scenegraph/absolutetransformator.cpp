
#include <core/scenegraph/absolutetransformator.h>

#include <typeinfo>
#include <assert.h>

#include <glm/glm.hpp>

#include <core/scenegraph/node.h>
#include <core/scenegraph/group.h>
#include <core/scenegraph/polygonaldrawable.h>
#include <core/scenegraph/polygonalgeometry.h>
#include <core/scenegraph/triangleobject.h>


AbsoluteTransformator::AbsoluteTransformator()
{
}

AbsoluteTransformator::~AbsoluteTransformator()
{
}

Node *AbsoluteTransformator::toAbsoluteIgnoreRootTransform(Node *relativeRoot) {
    return toAbsolute(relativeRoot, relativeRoot->transformInverse());
}

Node *AbsoluteTransformator::toAbsolute(Node *relativeRoot, glm::mat4 transform) {
    if (relativeRoot == nullptr) relativeRoot = new Group();
    glm::mat4 newTransform = relativeRoot->transform() * transform;
    Node *absoluteRoot;
    if (relativeRoot->children().size() > 0) {
        Group *absoluteGroup = new Group();
        for (auto child : relativeRoot->children()) {
            Node *absoluteChild = toAbsolute(static_cast<Node *>(child), newTransform);
            if (absoluteChild != nullptr) {
                absoluteGroup->append(absoluteChild);
            }
        }
        absoluteRoot = absoluteGroup;
    } else {
        PolygonalDrawable *poly = dynamic_cast<PolygonalDrawable *>(relativeRoot);
        if (poly == nullptr) {
            absoluteRoot = new Group();
        } else {
            auto relativeGeometry = poly->geometry();
            auto vertices = relativeGeometry->copyVertices();
            auto absoluteGeometry = new TriangleObject();
            p_TriangleList triangles = absoluteGeometry->triangles();

            for (auto index : relativeGeometry->indices()) {
                glm::vec4 homogenous = newTransform * glm::vec4(vertices.at(index), 1.0f);
                triangles->push_back(homogenous.xyz() * (1 / homogenous.w));
            }
            absoluteRoot = absoluteGeometry;
        }
    }
    absoluteRoot->setName(relativeRoot->name());
    absoluteRoot->setReferenceFrame(Node::RF_Absolute);
    return absoluteRoot;
}
