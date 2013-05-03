
#include <GL/glew.h>

#include <glm/gtc/matrix_transform.hpp>

#include <cassert>

#include <QApplication>
#include <QBasicTimer>

#include "canvas.h"
#include "abstractpainter.h"

#include <core/gpuquery.h>
#include <core/glformat.h>
#include <core/camera.h>


Canvas::Canvas(
    const GLFormat & format,
    QWidget * parent)

:   QGLWidget(format.asQGLFormat(), parent)
,   m_format(format)
,   m_timer(nullptr)
,   m_painter(nullptr)
{
    m_timer = new QBasicTimer();

    m_timer->start(format.vsync() ? 1000/60 : 0, this);

    setMinimumSize(1, 1);

    // Important for overdraw, not occluding the scene.
    setAutoFillBackground(false); 
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

    // This is required e.g. for overlay painting

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

#ifdef QT_OPENGL_ES
    glOrthof(0.f, 1.f, 0.f, 1.f, 0.f, 1.f);
 #else
    glOrtho(0.0, 1.0, 0.0, 1.0, 0.0, 1.0);
 #endif

    glMatrixMode(GL_MODELVIEW);
}

void Canvas::resizeGL(
    int width
,   int height)
{
    updateViewport();
    
    if(m_painter)
        m_painter->resize(width, height);
}


// http://doc.qt.digia.com/qt/opengl-overpainting-glwidget-cpp.html
// http://harmattan-dev.nokia.com/docs/library/html/qt4/opengl-overpainting.html

void Canvas::paintEvent(QPaintEvent *)
{
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    paint();

    glPopMatrix();
    glPopAttrib();

    // The fixed function OGL is used to support old school OpenGL calls for overlay painting.
    // NOTE: it is not tested, what happens on contexts in newer core profiles.

    QPainter painter(this);
    paintOverlay(painter);
    painter.end();
}

void Canvas::paint()
{
    if(m_painter)
        m_painter->paint();
    else 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

const QImage Canvas::capture(
    const bool alpha)
{
    if(!m_painter)
        return QImage();

    Camera* camera = m_painter->getCamera();

    if(!camera)
        return QImage();

    // aspect is false, since this accesses the cameras projection matrix with same aspect...
    return capture(QSize(camera->viewport().x, camera->viewport().y), false, alpha);
}

const QImage Canvas::capture(
    const QSize & size
,   const bool aspect
,   const bool alpha)
{
    static const GLuint tileW(1024);
    static const GLuint tileH(1024);

    if(!m_painter)
        return QImage();

    Camera* camera = m_painter->getCamera();

    if(!camera)
    {
        qWarning("No camera for frame capture available.");
        return QImage();
    }

    const GLuint w(camera->viewport().x);
    const GLuint h(camera->viewport().y);

    const GLuint frameW = size.width();
    const GLuint frameH = size.height();

    const glm::mat4 proj(aspect ? glm::perspective(camera->fovy()
        , static_cast<float>(frameW) / static_cast<float>(frameH)
        , camera->zNear(), camera->zFar()) : camera->projection());

    const glm::mat4 view(camera->view());

    const glm::vec4 viewport(0, 0, frameW, frameH);

    QImage frame(frameW, frameH, alpha ? QImage::Format_ARGB32 : QImage::Format_RGB888);
    QImage tile(tileW, tileH, alpha ? QImage::Format_ARGB32 : QImage::Format_RGB888);

    QPainter p(&frame);

    resize(tileW, tileH);
    camera->update();

    for (GLuint y = 0; y < frameH; y += tileH)
    for (GLuint x = 0; x < frameW; x += tileW)
    {
        const glm::mat4 pick = glm::pickMatrix(glm::vec2(x + tileW / 2,  y + tileH / 2),
                                               glm::vec2(tileW, tileH), viewport);

        const glm::mat4 projTile(pick * proj);

        camera->setTransform(projTile * view);

        paint();

        glReadPixels(0, 0, tileW, tileH, alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, tile.bits());
        p.drawImage(x, y, tile);
    }
    p.end();

    resize(w, h);
    camera->setTransform(proj * view);

    return frame.mirrored(false, true); // flip vertically
}
