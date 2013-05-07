#include "arcballnavigation.h"


ArcballNavigation::ArcballNavigation(void)
{
    reset();
}

ArcballNavigation::~ArcballNavigation(void)
{

}


void ArcballNavigation::reset()
{
    m_center = glm::vec3(0, 0, 0);
    m_direction = glm::vec3(0, 0, -2);
    m_mouse_last = m_mouse_cur = glm::vec2(0, 0);
    m_arcball_on = false;
    m_viewMatrix = glm::lookAt(m_center + m_direction, m_center, glm::vec3( 0.f, 1.f, 0.f));
    m_viewMatrixInverted = glm::inverse(m_viewMatrix);
}


const glm::mat4 ArcballNavigation::viewMatrix() {
    if (m_mouse_cur != m_mouse_last) {
        if(m_arcball_on)
            updateArcball();
        if(m_zoom_on)
            updateZoom();

        m_viewMatrixInverted = glm::inverse(m_viewMatrix); // is needed in next call anyway
        m_mouse_last = m_mouse_cur;
    }
    return m_viewMatrix;
}

void ArcballNavigation::updateArcball()
{
    glm::vec3 va = getArcballVector(m_mouse_last);
    glm::vec3 vb = getArcballVector(m_mouse_cur);

    float angle = acos(std::min(1.0f, glm::dot(va, vb)));
    glm::vec3 axis_in_camera_coord = glm::cross(va, vb);

    glm::mat3 camera2world = glm::mat3(m_viewMatrixInverted);
    glm::vec3 axis_in_world_coord = camera2world * axis_in_camera_coord;
    m_viewMatrix = m_viewMatrix * glm::rotate(glm::mat4(), glm::degrees(angle), axis_in_world_coord);
}

/**
 * Get a normalized vector from the center of the virtual ball O to a
 * point P on the virtual ball surface, such that P is aligned on
 * screen's (X,Y) coordinates.  If (X,Y) is too far away from the
 * sphere, return the nearest point on the virtual ball surface.
 * source: http://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Arcball
 */
glm::vec3 ArcballNavigation::getArcballVector(glm::vec2 v) {
  // normalize mouse coordinates to [-1:1]
  glm::vec3 P = glm::vec3(1.0 * v.x/m_width * 2 - 1.0,
                          1.0 * v.y/m_height * 2 - 1.0,
                          0);
  P.y = -P.y;
  float P_squared = P.x * P.x + P.y * P.y;
  if (P_squared <= 1*1) // P is inside ball
      // calculate height in ball, small P_squared means P is near center -> high z value
      P.z = sqrt(1*1 - P_squared); 
  else
      P = glm::normalize(P);  // nearest point to the ball
  return P;
}

void ArcballNavigation::updateZoom()
{
    float delta = (m_mouse_cur - m_mouse_last).y / m_height; // normalized height difference
    glm::vec3 zoom_in_camera = glm::vec3(0, 0, -delta) * glm::mat3(m_viewMatrix) ; // TODO relative to z-buffer
    
    glm::mat4 translate = glm::translate(glm::mat4(), zoom_in_camera);
    m_viewMatrix = m_viewMatrix * translate;
}


void ArcballNavigation::mouseMoveEvent(QMouseEvent * event)
{
    if (m_arcball_on || m_zoom_on) {
        m_mouse_cur = glm::vec2(event->pos().x(), event->pos().y());
    }
}

void ArcballNavigation::mousePressEvent(QMouseEvent * event)
{
    m_arcball_on = event->button() == Qt::LeftButton;
    m_zoom_on = event->button() == Qt::RightButton;
    m_mouse_last = m_mouse_cur = glm::vec2(event->pos().x(), event->pos().y());
}

void ArcballNavigation::mouseReleaseEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton) {
        m_arcball_on = false;
    }
    if (event->button() == Qt::RightButton) {
        m_zoom_on = false;
    }
}

const glm::mat4 ArcballNavigation::viewMatrixInverted()
{
     return m_viewMatrixInverted;
}



