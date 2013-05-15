#pragma once

#include "abstractnavigation.h"

class CGSEE_API ArcballNavigation : public AbstractNavigation
{
public:
    ArcballNavigation(Camera * camera);
    ~ArcballNavigation(void);
    
    virtual void mouseMoveEvent(QMouseEvent * event) override;
    virtual void mousePressEvent(QMouseEvent * event) override;
    virtual void mouseReleaseEvent(QMouseEvent * event) override;
    virtual void wheelEvent(QWheelEvent * event) override;

    const glm::mat4 viewMatrix() override;
    //virtual const glm::mat4 viewMatrixInverted(); 

private: 
    void updateArcball(void); 
    glm::vec3 getArcballVector(glm::vec2 v);
    void updateZoom(void);

private:
    glm::vec2 m_mouse_last;
    glm::vec2 m_mouse_cur;
    bool m_arcball_on;
    bool m_zoom_on;
};

