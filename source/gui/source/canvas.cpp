
#include <gui/canvas.h>

#include <cassert>

#include <glm/gtc/matrix_transform.hpp>

#include <QApplication>
#include <QBasicTimer>
#include <QSize>
#include <QDebug>

#include <glbinding/gl/types.h>
#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

#include <glow/glow.h>
#include <glow/Error.h>

#include <core/navigation/navigationhistory.h>
#include <core/navigation/abstractnavigation.h>

#include <core/camera/abstractcamera.h>
#include <core/camera/projection.h>

#include <core/painter/abstractpainter.h>
#include <core/painter/abstractscenepainter.h>

#include <core/abstracteventhandler.h>
#include <core/gpuquery.h>
#include <core/glformat.h>
#include <core/timer.h>


Canvas::Canvas(
    const GLFormat & format,
    QWidget * parent)

:   QGLWidget(format.asQGLFormat(), parent)
,   m_refreshTimeMSec(0)
,   m_painter(nullptr)
,   m_navigationHistory(nullptr)
,   m_eventHandler(nullptr)
,   m_timer(new QBasicTimer)
,   m_format(format)
{
    setMinimumSize(1, 1);
    // Important for overdraw, not occluding the scene.
    setAutoFillBackground(false);

    m_navigationHistory = new NavigationHistory();
}

Canvas::~Canvas()
{
    delete m_timer;
}

void Canvas::initializeGL()
{
    glError();  // nothing should be done before this!

    if(!context()->isValid())
        qCritical("Qt OpenGL context is not valid.");

    qDebug("Vendor: %s", qPrintable(GPUQuery::vendor()));
    qDebug("Renderer: %s", qPrintable(GPUQuery::renderer()));
    qDebug("Version: %s", qPrintable(GPUQuery::version()));

    //qDebug("GLEW Version: %s\n", qPrintable(GPUQuery::glewVersion()));

    // NOTE : Important for e.g., 3.2 core
    // http://glew.sourceforge.net/basic.html

    glError();

    if (!glow::init())
    {
        qCritical("Glow failed to initialized: %s\n", qPrintable(QString::fromStdString(glow::Error::get().name())));
    }

    //if(!GPUQuery::isCoreProfile())
    //{
    //    qDebug("Memory (total):     %i MiB", GPUQuery::totalMemory() / 1024);
    //    qDebug("Memory (available): %i MiB\n", GPUQuery::availableMemory() / 1024);
    //}

    gl::glClearColor(1.f, 1.f, 1.f, 1.f);
    glError();
}

void Canvas::resizeGL(
    int width
,   int height)
{
    if(m_painter)
        m_painter->resize(width, height);

    if(m_eventHandler)
        m_eventHandler->resize(size());
}

void Canvas::paintGL()
{
    glError();

    if(m_painter)
        m_painter->paint();
    else
        gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);

    glError();
}

void Canvas::timerEvent(QTimerEvent * event)
{
    assert(m_timer);
    if(event->timerId() != m_timer->timerId())
        return;

    update();
}

void Canvas::setRefreshTimeMSec(int msec)
{
    m_refreshTimeMSec = msec;
    if (msec < 0)
        m_timer->stop();
    else
        m_timer->start(m_refreshTimeMSec, this);
}

int Canvas::refreshTimeMSec() const
{
    return m_refreshTimeMSec;
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
    // aspect is false, since this accesses the cameras projection matrix with same aspect...
    return capture(size(), false, alpha);
}

const QImage Canvas::capture(
    const QSize & size
,   const bool aspect
,   const bool alpha)
{
    if (!m_painter)
    {
        qWarning("No painter for frame capture available.");
        return QImage();
    }

    AbstractScenePainter * scenePainter = dynamic_cast<AbstractScenePainter *>(m_painter);
    if(!scenePainter)
    {
        qWarning("Painter does not provide a camera for frame capture.");
        return QImage();
    }
    AbstractCamera * camera(scenePainter->camera());
    if (!camera)
    {
        qWarning("No camera for frame capture available.");
        return QImage();
    }
    //TODO move camera responsibility to painter->paintTile(width,height)
    //TODO keep own fbo to avoid drawing on screen
    static const gl::GLuint tileW(512);
    static const gl::GLuint tileH(512);

    const gl::GLuint w(camera->viewport().x);
    const gl::GLuint h(camera->viewport().y);

    const gl::GLuint frameW = size.width();
    const gl::GLuint frameH = size.height();

    Projection * projection = new Projection(*camera->getProjection());

    const glm::mat4 proj(aspect ? glm::perspective(camera->fovy()
        , static_cast<float>(frameW) / static_cast<float>(frameH)
        , camera->zNear(), camera->zFar()) : camera->projection());

    const glm::mat4 view(camera->view());

    const glm::vec4 viewport(0, 0, frameW, frameH);

    QImage frame(frameW, frameH, alpha ? QImage::Format_ARGB32 : QImage::Format_RGB888);
    QImage tile(tileW, tileH, alpha ? QImage::Format_ARGB32 : QImage::Format_RGB888);

    QPainter p(&frame);

    // ToDo: Review after painter refactoring
    makeCurrent();

    m_painter->resize(tileW, tileH);
    camera->setViewport(tileW, tileH);


    for (gl::GLuint y = 0; y < frameH; y += tileH)
    for (gl::GLuint x = 0; x < frameW; x += tileW)
        {
        const glm::mat4 pick = glm::pickMatrix(glm::vec2(x + tileW / 2, y + tileH / 2),
            glm::vec2(tileW, tileH), viewport);

        const glm::mat4 projTile(pick * proj);

        camera->setTransform(projTile * view);

        m_painter->paint();

        glReadPixels(0, 0, tileW, tileH, alpha ? gl::GL_RGBA : gl::GL_RGB, gl::GL_UNSIGNED_BYTE, tile.bits());
        p.drawImage(x, y, tile);
        }
    p.end();

    resizeGL(w, h);
    Projection * tempProjection = camera->getProjection();
    camera->setProjection(projection);
    delete tempProjection;
    camera->setView(view);

    doneCurrent();

    return frame.mirrored(false, true); // flip vertically
}

void Canvas::resize(int width, int height)
{
    QGLWidget::resize(width, height);
}

AbstractEventHandler * Canvas::eventHandler()
{
    return m_eventHandler;
}

void Canvas::setEventHandler(AbstractEventHandler * eventHandler)
{
	if (eventHandler == m_eventHandler)
	return;

	m_eventHandler = eventHandler;
	m_eventHandler->resize(size());

    /// Links navigation and navigation history and the view changed signal.
    m_navigationHistory->setNavigation(dynamic_cast<AbstractNavigation*>(m_eventHandler));
    dynamic_cast<AbstractNavigation*>(m_eventHandler)->viewChanged.connect(this, &Canvas::saveHistory);

}

/**
 * @brief Getter for navigation history.
 * @details Allows access to the whole navigation history.
 * @return The navigation history.
 */
NavigationHistory * Canvas::navigationHistory()
{
    return m_navigationHistory;
}

/**
 * @brief Saves the current view to history.
 * @details Saves the current view to navigation history including an image of
 *          the current canvas.
 *
 * @param viewmatrix The current view matrix.
 * @param fovy The current field of view.
 */
void Canvas::saveHistory(glm::mat4 viewmatrix, float fovy)
{
    //m_navigationHistory->save(viewmatrix, fovy, capture(QSize(512, 512), true, false));
}

void Canvas::mousePressEvent(QMouseEvent * event)
{
    if (m_eventHandler)
        m_eventHandler->mousePressEvent(event);
}

void Canvas::mouseReleaseEvent(QMouseEvent * event)
{
    if (m_eventHandler)
        m_eventHandler->mouseReleaseEvent(event);

	emit mouseReleaseEventSignal(event);
}

void Canvas::mouseMoveEvent(QMouseEvent * event)
{
    this->setFocus();
    
    if (m_eventHandler)
        m_eventHandler->mouseMoveEvent(event);

	emit mouseMoveEventTriggered(1);
}

void Canvas::wheelEvent(QWheelEvent * event)
{
    if (m_eventHandler)
        m_eventHandler->wheelEvent(event);
}
