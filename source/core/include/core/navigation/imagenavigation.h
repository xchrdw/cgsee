#pragma once

#include <core/core_api.h>

#include <core/navigation/abstractnavigation.h>
#include <glm/gtc/matrix_transform.hpp>

class ImagePainter;

class CORE_API ImageNavigation : public AbstractNavigation
{
public:
    ImageNavigation();
    ~ImageNavigation();
    
    virtual void mouseMoveEvent(QMouseEvent * event) override;
    virtual void mousePressEvent(QMouseEvent * event) override;
    virtual void mouseReleaseEvent(QMouseEvent * event) override;

    virtual void wheelEvent(QWheelEvent *event);

    void setPainter(ImagePainter* painter);

private: 
    glm::vec3 getArcballVector(glm::vec2 v);
    void updateArcball(); 
    void updateZoom();
    void updatePanning();

private:
    glm::vec2 m_mouse_last;
    glm::vec2 m_mouse_cur;
    bool m_arcball_on;
    bool m_zoom_on;
    bool m_panning_on;

    ImagePainter* m_painter;
};
