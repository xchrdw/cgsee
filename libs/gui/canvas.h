#pragma once

#include <QGLWidget>

#include <core/declspec.h>
#include <core/glformat.h>


class QBasicTimer;
class QTimerEvent;
class QMouseEvent;
class AbstractPainter;

class CGSEE_API Canvas : public QGLWidget
{
public:
    Canvas(const GLFormat & format, QWidget * parent = nullptr);
    virtual ~Canvas();

    void setPainter(AbstractPainter * painter);
    AbstractPainter * painter();

protected:
    virtual void initializeGL();
    virtual void resizeGL(int width, int height);
    virtual void paintGL();

    void paint();
    void paintOverlay(QPainter & painter);
    void timerEvent(QTimerEvent * event);
    void mousePressEvent(QMouseEvent * event);
    void updateViewport() const;

protected:
    AbstractPainter * m_painter;
    QBasicTimer * m_timer;
    const GLFormat m_format;
};
