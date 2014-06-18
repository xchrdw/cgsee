#pragma once

#include <core/core_api.h>

#include <QObject>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QMouseEvent>

class CORE_API AbstractNavigation : public QObject 
{
    
public:
    AbstractNavigation();
    virtual ~ AbstractNavigation();

    // interaction mapping
    virtual void keyPressEvent(QKeyEvent * event);
    virtual void keyReleaseEvent(QKeyEvent * event);

    virtual void mouseMoveEvent(QMouseEvent * event);
    virtual void mousePressEvent(QMouseEvent * event);
    virtual void mouseReleaseEvent(QMouseEvent * event);
    virtual void mouseDoubleClickEvent(QMouseEvent * event);
    virtual void wheelEvent(QWheelEvent *event);

    void setCanvas(QWidget * canvas);
    void setViewPort(const int width, const int height);

protected:
    int m_width;
    int m_height;

    QWidget * m_canvas;
};
