
#include <core/viewfrustum.h>

#include <core/aabb.h>
#include <core/scenegraph/node.h>
#include <core/scenegraph/group.h>
#include <core/camera.h>
#include <core/plane.h>

ViewFrustum::ViewFrustum(Camera *camera) :
    m_camera(camera),
    m_planes(),
    m_vertices(),
    m_dirty(true)
{
}

ViewFrustum::e_insideFrustum ViewFrustum::contains(const AxisAlignedBoundingBox &aabb, glm::mat4 transform) const {

    bool sphereCompletelyInsideFrustum = true;

    // test bounding sphere:
    for (auto plane : m_planes) {
        // distance of center to plane =
        //   < point - plane_origin_dist * normal ; normal > = 
        //   < point ; normal > - plane_origin_dist
        float centerDist = glm::dot(aabb.center(), plane.normal()) - plane.distance();
        if (centerDist <= aabb.radius()) {
            return OUTSIDE_FRUSTUM;
        } else if (centerDist >= -aabb.radius()) {
            continue;
        } else {
            sphereCompletelyInsideFrustum = false;
            break;
            //alternative to break:
            //return INTERSECTS_FRUSTUM;
            // we don't do this because we continue with a more precise test 
            // against the bounding box
            // TODO: is this efficient? Maybe we should do only the test against the bounding box 
            // if the corresponding node does not have subnodes or if the quotient
            // volume / radius is small
        }
    }
    if (sphereCompletelyInsideFrustum) {
        return INSIDE_FRUSTUM;
    } //else: sphere intersects frustum

    // test bounding box:
    bool contains_at_least_1_point = false;
    bool contains_at_most_7_points = false;
    for (auto aabbVertex : aabb.allVertices()) {
        glm::vec4 homogenousAABBVertex = transform * glm::vec4(aabbVertex, 1.0);
        if (this->contains((1 / homogenousAABBVertex.w) * homogenousAABBVertex.xyz)) {
            contains_at_least_1_point = true;
            if (contains_at_most_7_points) {
                return INTERSECTS_FRUSTUM;
            }
        } else {
            contains_at_most_7_points = true;
            if (contains_at_least_1_point) {
                return INTERSECTS_FRUSTUM;
            }
        }
    }
    // contains_at_least_1_point and contains_at_most_7_points can't both be true 
    // in this case the function must have returned INTERSECTS_FRUSTUM yet
    if (contains_at_least_1_point) {
        return INSIDE_FRUSTUM;
    } 
    for (auto vertex : this->m_vertices) {
        if (aabb.inside(vertex)) {
            return INTERSECTS_FRUSTUM;
        }
    }
    return OUTSIDE_FRUSTUM;    
}

bool ViewFrustum::contains(glm::vec3 point) const {
    for (auto plane : m_planes) {
        // < point - dist * normal ; normal > = < point ; normal > - dist >
        // <= 0: outside
        if (glm::dot(point, plane.normal()) - plane.distance() <= 0 ) {
            return false;
        }
    }
    return true;
}

void ViewFrustum::update() {
    glm::mat4 projection = m_camera->projection();
    m_planes[0] = Plane(glm::vec3(0.0, 0.0,  1.0), 1, projection);
    m_planes[1] = Plane(glm::vec3(0.0, 0.0, -1.0), 1, projection);
    m_planes[2] = Plane(glm::vec3(0.0,  1.0, 0.0), 1, projection);
    m_planes[3] = Plane(glm::vec3(0.0, -1.0, 0.0), 1, projection);
    m_planes[4] = Plane(glm::vec3( 1.0, 0.0, 0.0), 1, projection);
    m_planes[5] = Plane(glm::vec3(-1.0, 0.0, 0.0), 1, projection);
    glm::vec4 homogenous;
    homogenous = projection * glm::vec4( 1.0,  1.0,  1.0,  1.0);
    m_vertices[0] = glm::vec3((1 / homogenous.w) * homogenous.xyz);
    homogenous = projection * glm::vec4(-1.0,  1.0,  1.0,  1.0);
    m_vertices[1] = glm::vec3((1 / homogenous.w) * homogenous.xyz);
    homogenous = projection * glm::vec4( 1.0, -1.0,  1.0,  1.0);
    m_vertices[2] = glm::vec3((1 / homogenous.w) * homogenous.xyz);
    homogenous = projection * glm::vec4(-1.0, -1.0,  1.0,  1.0);
    m_vertices[3] = glm::vec3((1 / homogenous.w) * homogenous.xyz);
    homogenous = projection * glm::vec4( 1.0,  1.0, -1.0,  1.0);
    m_vertices[4] = glm::vec3((1 / homogenous.w) * homogenous.xyz);
    homogenous = projection * glm::vec4(-1.0,  1.0, -1.0,  1.0);
    m_vertices[5] = glm::vec3((1 / homogenous.w) * homogenous.xyz);
    homogenous = projection * glm::vec4( 1.0, -1.0, -1.0,  1.0);
    m_vertices[6] = glm::vec3((1 / homogenous.w) * homogenous.xyz);
    homogenous = projection * glm::vec4(-1.0, -1.0, -1.0,  1.0);
    m_vertices[7] = glm::vec3((1 / homogenous.w) * homogenous.xyz);
    m_dirty = false;
}

bool ViewFrustum::isDirty() const {
    return m_dirty;
}

void ViewFrustum::dirty() {
    m_dirty = true;
}

ViewFrustum::~ViewFrustum() {
    
}
