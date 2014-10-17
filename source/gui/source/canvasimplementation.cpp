#include <gui/canvasimplementation.h>

#include <QWindow>
#include <QOpenGLContext>
#include <QSurface>
#include <QEvent>

#include <core/glformat.h>

CanvasImplementation::CanvasImplementation(const GLFormat & format)
:   QWindow()
{
    setSurfaceType(QSurface::OpenGLSurface);
    setFormat(format.asQSurfaceFormat());
	create();

    m_context = new QOpenGLContext(this);
    m_context->setFormat(format.asQSurfaceFormat());
    m_context->create();
}

CanvasImplementation::~CanvasImplementation(){
    //Qt should take care of deletion, because m_context is child of this
}

void CanvasImplementation::makeCurrent()
{
    m_context->makeCurrent(this);
}

void CanvasImplementation::doneCurrent()
{
    if (QOpenGLContext::currentContext() == m_context)
        m_context->doneCurrent();
}

void CanvasImplementation::swapBuffers()
{
    m_context->swapBuffers(this);
}

void CanvasImplementation::paint()
{
    if (isExposed())
    {
        makeCurrent();
        emit renderingRequested();
        swapBuffers();
    }
}

void CanvasImplementation::resizeEvent(QResizeEvent * event)
{
    QWindow::resizeEvent(event);
    paint();
}

void CanvasImplementation::exposeEvent(QExposeEvent * event)
{
    QWindow::exposeEvent(event);
    paint();
}

bool CanvasImplementation::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::UpdateRequest:
        paint();
        return true;
    default:
        return QWindow::event(event);
    }
}