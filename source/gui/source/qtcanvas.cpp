#include <gui/qtcanvas.h>

#include <QWindow>
#include <QOpenGLContext>
#include <QSurface>
#include <QEvent>

#include <core/glformat.h>

QtCanvas::QtCanvas(const GLFormat & format)
:   QWindow()
{
    setSurfaceType(QSurface::OpenGLSurface);
    setFormat(format.asQSurfaceFormat());    
    create();

    m_context = new QOpenGLContext(this);
    m_context->setFormat(format.asQSurfaceFormat());
    m_context->create();
}

QtCanvas::~QtCanvas(){
    //Qt should take care of deletion, because m_context is child of this
}

void QtCanvas::makeCurrent()
{
    m_context->makeCurrent(this);
}

void QtCanvas::doneCurrent()
{
    if (QOpenGLContext::currentContext() == m_context)
        m_context->doneCurrent();
}

void QtCanvas::swapBuffers()
{
    m_context->swapBuffers(this);
}

void QtCanvas::paint()
{
    if (isExposed())
    {
        makeCurrent();
        emit renderingRequested();
        swapBuffers();
    }
}

void QtCanvas::resizeEvent(QResizeEvent * event)
{
    paint();
}

void QtCanvas::exposeEvent(QExposeEvent * event)
{
    QWindow::exposeEvent(event);
    paint();
}

bool QtCanvas::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::UpdateRequest:
        paint();
        return true;
    default:
        return QWindow::event(event);
    }
}