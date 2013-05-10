
#pragma once

#include <QGLWidget>

#include <core/declspec.h>
#include <core/glformat.h>

class QBasicTimer;
class QTimerEvent;

class AbstractPainter;


class CGSEE_API Canvas : public QGLWidget
{
public:
    Canvas(
        const GLFormat & format,
        QWidget * parent = nullptr);
    virtual ~Canvas();

    void setPainter(AbstractPainter * painter);
    AbstractPainter * painter();

    const QImage capture(
        const bool alpha = false);
    const QImage capture(
        const QSize & size
    ,   const bool aspect = true
    ,   const bool alpha = false);

protected:
    
    // QGLWidget Interface
    virtual void initializeGL();
    virtual void resizeGL(
        int width
    ,   int height);

    // http://doc.qt.digia.com/qt/opengl-overpainting.html
    // "Instead of implementing paintGL() to handle updates to the
    //  widget, we implement a normal QWidget::paintEvent(). This
    //  allows us to mix OpenGL calls and QPainter operations in a
    //  controlled way."
    
    //virtual void paintEvent(QPaintEvent * event); // Does not work for 3.2 anyway. :(
    //void paintOverlay(QPainter & painter);

    virtual void paintGL();

    // For Rendering Loop
    void timerEvent(QTimerEvent *event);

    void updateViewport() const;

protected:
    AbstractPainter * m_painter;

    QBasicTimer * m_timer;
    const GLFormat m_format;
};
