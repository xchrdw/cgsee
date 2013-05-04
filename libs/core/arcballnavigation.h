#pragma once
#include "abstractnavigation.h"

class ArcballNavigation : public AbstractNavigation
{
public:
	ArcballNavigation(void);
	~ArcballNavigation(void);
	
    virtual void mouseMoveEvent(const glm::vec2 point);
    virtual void mousePressEvent(const glm::vec2 point, MouseButton button);
    virtual void mouseReleaseEvent(const glm::vec2 point, MouseButton button);

    virtual const glm::mat4 viewMatrix();
    virtual void reset();
	
private: 
    glm::vec3 get_arcball_vector(glm::vec2 v);

    glm::vec3 m_center;

    glm::vec2 m_last;
    glm::vec2 m_cur;
    bool m_arcball_on;

    int m_screen_width;
    int m_screen_height;

    glm::mat4 m_viewMatrix;

    // source: http://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Arcball
};

