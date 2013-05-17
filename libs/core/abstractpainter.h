
#pragma once

#include <QStringList>

#include <core/declspec.h>
#include <core/glformat.h>

class QKeyEvent;
class Camera;
class AbstractGLParent;

class CGSEE_API AbstractPainter
{
public:
    AbstractPainter(Camera * camera);
    virtual ~AbstractPainter();

    // calls initialize if not initialized - so call this when subclassed
    virtual void paint();

    const QImage capture(
        AbstractGLParent & parent
    ,   const QSize & size
    ,   const bool aspect
    ,   const bool alpha);

    virtual void resize(
        const int width
    ,   const int height);

    Camera * camera();

    /*virtual void keyPress(QKeyEvent * event);
    virtual void keyRelease(QKeyEvent * event);
    virtual void mouseMoveEvent (QMouseEvent * event);
    virtual void mousePressEvent (QMouseEvent * event);
    virtual void mouseReleaseEvent (QMouseEvent * event);
    virtual void wheelEvent(QWheelEvent * event);*/

    // TODO: more to come...

protected:
    virtual const bool initialize() = 0;

protected:
    bool m_initialized;
    Camera * m_camera;
};
