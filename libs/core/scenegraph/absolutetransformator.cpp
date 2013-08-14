#include <typeinfo>
#include <assert.h>

#include <glm/glm.hpp>

#include "core/declspec.h"

#include "node.h"
#include "group.h"
#include "polygonaldrawable.h"
#include "polygonalgeometry.h"

#include "absolutetransformator.h"

AbsoluteTransformator::AbsoluteTransformator(std::shared_ptr<DataBlockRegistry> registry)
: m_registry(registry)
{
}

AbsoluteTransformator::~AbsoluteTransformator()
{
}

Node *AbsoluteTransformator::toAbsolute(Node *relativeRoot) {
    return toAbsolute(relativeRoot, glm::inverse(relativeRoot->transform()));
}

Node *AbsoluteTransformator::toAbsolute(Node *relativeRoot, glm::mat4 transform) {
    if (relativeRoot == nullptr) return nullptr;
    glm::mat4 newTransform = relativeRoot->transform() * transform;
    Node *absoluteRoot;
    //if (typeid(*relativeRoot) == typeid(Group)) {
    if (relativeRoot->children().size() > 0) {
        Group *absoluteGroup = new Group();
        for (auto child : relativeRoot->children()) {
            absoluteGroup->append(toAbsolute(child, newTransform));
        }
        absoluteRoot = absoluteGroup;
    //} else if (typeid(*relativeRoot) == typeid(PolygonalDrawable)) {
    } else {
        PolygonalDrawable *poly = dynamic_cast<PolygonalDrawable *>(relativeRoot);
        if (poly == nullptr) {
            absoluteRoot = new Group();
        } else {
            auto relativeGeometry = dynamic_cast<PolygonalDrawable *>(relativeRoot)->geometry(); //fails
            auto absoluteGeometry = std::shared_ptr<PolygonalGeometry>(new PolygonalGeometry(m_registry));
            auto vertices = relativeGeometry->copyVertices();
        
            for (int i = 0; i < vertices.size(); i++) {
                glm::vec4 homogenous = newTransform * glm::vec4(vertices.at(i), 1.0f);
                absoluteGeometry->setVertex(i, glm::vec3(homogenous) / homogenous.w);
            }
            for (int i = 0; i < relativeGeometry->indices().size(); ++i) {
                absoluteGeometry->setIndex(i, relativeGeometry->indices().at(i));
            }
            PolygonalDrawable * drawable = new PolygonalDrawable();
            drawable->setGeometry(absoluteGeometry);
            drawable->setMode(GL_TRIANGLES);
            absoluteRoot = drawable;
        }
    //} else {
    //    assert (false); //this should never happen
    //    absoluteRoot = new Group();
    }
    absoluteRoot->setName(relativeRoot->name());
    absoluteRoot->setReferenceFrame(Node::RF_Absolute);
    return absoluteRoot;
}