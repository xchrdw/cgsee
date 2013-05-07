#pragma once

#include "abstractnavigation.h"

class CGSEE_API ArcballNavigation : public AbstractNavigation
{
public:
	ArcballNavigation(void);
	~ArcballNavigation(void);
	
    virtual void mouseMoveEvent(QMouseEvent * event);
    virtual void mousePressEvent(QMouseEvent * event);
    virtual void mouseReleaseEvent(QMouseEvent * event);

    virtual const glm::mat4 viewMatrix();



    virtual const glm::mat4 viewMatrixInverted();
    virtual void reset();
	
private: 
    void updateArcball(); 
    glm::vec3 getArcballVector(glm::vec2 v);
    void updateZoom();

    glm::vec3 m_center;
    glm::vec3 m_direction;

    glm::vec2 m_mouse_last;
    glm::vec2 m_mouse_cur;
    bool m_arcball_on;
    bool m_zoom_on;

    glm::mat4 m_viewMatrix;
    glm::mat4 m_viewMatrixInverted;
};

