#include "arcballnavigation.h"


ArcballNavigation::ArcballNavigation(void)
{
    reset();
}

void ArcballNavigation::reset()
{
    m_center = glm::vec3(0, 0, 0);
    m_last = m_cur = glm::vec2(0, 0);
    m_arcball_on = false;
}


ArcballNavigation::~ArcballNavigation(void)
{
}

const glm::mat4 ArcballNavigation::viewMatrix() {
    if (m_cur != m_last) {
        glm::vec3 va = get_arcball_vector(m_last);
        glm::vec3 vb = get_arcball_vector(m_cur);
        float angle = acos(std::min(1.0f, glm::dot(va, vb)));
        glm::vec3 axis_in_camera_coord = glm::cross(va, vb);
        //glm::mat3 camera2object = glm::inverse(glm::mat3(transforms[MODE_CAMERA]) * glm::mat3(mesh.object2world));
        //glm::vec3 axis_in_object_coord = camera2object * axis_in_camera_coord;
        //mesh.object2world = glm::rotate(mesh.object2world, glm::degrees(angle), axis_in_object_coord);
        m_last = m_cur;
    }
    return m_viewMatrix;
}

/**
 * Get a normalized vector from the center of the virtual ball O to a
 * point P on the virtual ball surface, such that P is aligned on
 * screen's (X,Y) coordinates.  If (X,Y) is too far away from the
 * sphere, return the nearest point on the virtual ball surface.
 */
glm::vec3 ArcballNavigation::get_arcball_vector(glm::vec2 v) {
  glm::vec3 P = glm::vec3(1.0*v.x/m_screen_width*2 - 1.0,
                          1.0*v.y/m_screen_height*2 - 1.0,
                          0);
  P.y = -P.y;
  float OP_squared = P.x * P.x + P.y * P.y;
  if (OP_squared <= 1*1)
    P.z = sqrt(1*1 - OP_squared);
  else
    P = glm::normalize(P);  // nearest point
  return P;
}

void ArcballNavigation::mouseMoveEvent(const glm::vec2 point)
{
    if (m_arcball_on) {  // if left button is pressed
        m_cur = point;
    }
}

void ArcballNavigation::mousePressEvent( const glm::vec2 point, MouseButton button )
{
    if (button == LEFT) {
        m_arcball_on = true;
        m_last = m_cur = point;
    }
}

void ArcballNavigation::mouseReleaseEvent( const glm::vec2 point, MouseButton button )
{
    if (button == LEFT) {
        m_arcball_on = false;
    }
}
