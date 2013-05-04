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
    glm::vec3 get_arcball_vector(glm::vec2 v);

    glm::vec3 m_center;
    glm::vec3 m_direction;

    glm::vec2 m_last;
    glm::vec2 m_cur;
    bool m_arcball_on;

    glm::mat4 m_viewMatrix;

    // source: http://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Arcball
};

