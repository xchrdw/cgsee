#include <GL/glew.h>

#include <cassert>

#include <QApplication>
#include <QBasicTimer>

#include "canvas.h"
#include "abstractpainter.h"

#include <core/gpuquery.h>
#include <core/glformat.h>


Canvas::Canvas(const GLFormat & format, QWidget * parent)
:   QGLWidget(format.asQGLFormat(), parent)
,   m_format(format)
,   m_timer(nullptr)
,   m_painter(nullptr)
{
    m_timer = new QBasicTimer();
    m_timer->start(format.vsync() ? 1000/60 : 0, this);

    setMinimumSize(1, 1);
    setAutoFillBackground(false);
    setMouseTracking(true);
}

Canvas::~Canvas()
{
    delete m_timer;
}

void Canvas::initializeGL()
{
    if(!context()->isValid())
        qCritical("Qt OpenGL context is not valid.");

    qDebug("Vendor: %s", qPrintable(GPUQuery::vendor()));
    qDebug("Renderer: %s", qPrintable(GPUQuery::renderer()));
    qDebug("GLEW Version: %s\n", qPrintable(GPUQuery::glewVersion()));

    glewExperimental = GL_TRUE;
    const GLenum error = glewInit();
    if(GLEW_OK != error)
        qCritical("Glew failed to initialized: %s\n", qPrintable(GPUQuery::glewError(error)));

    if(!m_format.verify(context()->format()))
        qCritical("There might be problems during scene initialization and rendering.\n");

    qDebug("Memory (total):     %i MiB", GPUQuery::totalMemory() / 1024);
    qDebug("Memory (available): %i MiB\n", GPUQuery::availableMemory() / 1024);

    glClearColor(1.f, 1.f, 1.f, 1.f);
}

void Canvas::updateViewport() const
{
    glViewport(0, 0, width(), height());
}

void Canvas::resizeGL(int width, int height)
{
    updateViewport();

    if(m_painter)
        m_painter->resize(width, height);
}

void Canvas::paintGL()
{
    paint();
}

void Canvas::paint()
{
    if(m_painter)
        m_painter->paint();
}

void Canvas::paintOverlay(QPainter & painter)
{
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);
}

void Canvas::timerEvent(QTimerEvent *event)
{
    assert(m_timer);
    if(event->timerId() != m_timer->timerId())
        return;

    update();
}

void Canvas::mousePressEvent(QMouseEvent * event)
{
    if(m_painter)
        m_painter->objectDetected(mapFromGlobal(QCursor::pos()));
}

void Canvas::setPainter(AbstractPainter * painter)
{
    if(m_painter == painter)
        return;

    m_painter = painter;
    update();
}

AbstractPainter * Canvas::painter()
{
    return m_painter;
}
