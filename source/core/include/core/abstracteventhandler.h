#pragma once

#include <core/core_api.h>

class QKeyEvent;
class QWheelEvent;
class QMouseEvent;
class QResizeEvent;
class QSize;

class CORE_API AbstractEventHandler
{
public:
    virtual void keyPressEvent(QKeyEvent * event) = 0;
    virtual void keyReleaseEvent(QKeyEvent * event) = 0;

    virtual void mouseMoveEvent(QMouseEvent * event) = 0;
    virtual void mousePressEvent(QMouseEvent * event) = 0;
    virtual void mouseReleaseEvent(QMouseEvent * event) = 0;
    virtual void mouseDoubleClickEvent(QMouseEvent * event) = 0;

    virtual void wheelEvent(QWheelEvent * event) = 0;

    virtual void resizeEvent(QResizeEvent * event) = 0;
    virtual void resize(const QSize & size) = 0;
};
