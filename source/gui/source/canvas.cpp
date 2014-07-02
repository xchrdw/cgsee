
#include <gui/canvas.h>

#include <GL/glew.h>

#include <cassert>

#include <glm/gtc/matrix_transform.hpp>

#include <QApplication>
#include <QBasicTimer>
#include <QSize>
#include <QDebug>

#include <core/navigation/navigationhistory.h>
#include <core/navigation/abstractnavigation.h>
#include <core/camera.h>
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
,   m_camera(nullptr)
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

    qDebug("GLEW Version: %s\n", qPrintable(GPUQuery::glewVersion()));

    // NOTE : Important for e.g., 3.2 core
    // http://glew.sourceforge.net/basic.html

    glError();
    glewExperimental = GL_TRUE;
    const GLenum error = glewInit();

    // http://stackoverflow.com/questions/10857335/opengl-glgeterror-returns-invalid-enum-after-call-to-glewinit
    // use glGetError instead of userdefined glError, to avoid console/log output
    glGetError();
    glError();

    if(GLEW_OK != error)
        qCritical("Glew failed to initialized: %s\n", qPrintable(GPUQuery::glewError(error)));

    if(!m_format.verify(context()->format()))
        qWarning("There might be problems during scene initialization and rendering.\n");

    glError();

    if(!GPUQuery::isCoreProfile())
    {
        qDebug("Memory (total):     %i MiB", GPUQuery::totalMemory() / 1024);
        qDebug("Memory (available): %i MiB\n", GPUQuery::availableMemory() / 1024);
    }

    glClearColor(1.f, 1.f, 1.f, 1.f);
    glError();
}

void Canvas::resizeGL(
    int width
,   int height)
{
    if (m_camera)
        m_camera->setViewport(width, height);

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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
    // ToDo: refine AbstractScenePainter .. or wait for painter refactoring ;)
    // TODO BEGIN
    AbstractScenePainter * scenePainter(dynamic_cast<AbstractScenePainter*>(painter));

    if (painter && !scenePainter)
    {
        qDebug() << "Canvas requires a painter that is aware of cameras (uses a camera as entrypoint into the scenegraph).";
        return;
    }
	
    //if (m_camera)
    //    scenePainter->assignScene(m_camera);
    // TODO END

    if(m_painter == painter)
        return;

    m_painter = painter;
    update();
}

AbstractPainter * Canvas::painter()
{
    return m_painter;
}

void Canvas::setCamera(Camera * camera)
{
    if(m_camera == camera)
        return;

	/*
    if (m_painter)
    {
        // ToDo: see setPainter
        AbstractScenePainter * scenePainter(dynamic_cast<AbstractScenePainter*>(m_painter));
        scenePainter->assignScene(m_camera);
    }
	*/

    m_camera = camera;
    //update();
}

Camera * Canvas::camera()
{
    return m_camera;
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
    if (!m_camera)
    {
        qWarning("No camera for frame capture available.");
        return QImage();
    }

    static const GLuint tileW(512);
    static const GLuint tileH(512);

    const GLuint w(m_camera->viewport().x);
    const GLuint h(m_camera->viewport().y);

    const GLuint frameW = size.width();
    const GLuint frameH = size.height();

    const glm::mat4 proj(aspect ? glm::perspective(m_camera->fovy()
        , static_cast<float>(frameW) / static_cast<float>(frameH)
        , m_camera->zNear(), m_camera->zFar()) : m_camera->projection());

    const glm::mat4 view(m_camera->view());

    const glm::vec4 viewport(0, 0, frameW, frameH);

    QImage frame(frameW, frameH, alpha ? QImage::Format_ARGB32 : QImage::Format_RGB888);
    QImage tile(tileW, tileH, alpha ? QImage::Format_ARGB32 : QImage::Format_RGB888);

    QPainter p(&frame);

    // ToDo: Review after painter refactoring
    makeCurrent();

    m_painter->resize(tileW, tileH);
    m_camera->setViewport(tileW, tileH);
    m_camera->update(); // ToDo: required?


    for (GLuint y = 0; y < frameH; y += tileH)
        for (GLuint x = 0; x < frameW; x += tileW)
        {
        const glm::mat4 pick = glm::pickMatrix(glm::vec2(x + tileW / 2, y + tileH / 2),
            glm::vec2(tileW, tileH), viewport);

        const glm::mat4 projTile(pick * proj);

        m_camera->setTransform(projTile * view);

        m_painter->paint();

        glReadPixels(0, 0, tileW, tileH, alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, tile.bits());
        p.drawImage(x, y, tile);
        }
    p.end();

	resizeGL(w, h);
    m_camera->setTransform(proj * view);

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
    m_navigationHistory->save(viewmatrix, fovy, capture(QSize(512, 512), true, false));
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
