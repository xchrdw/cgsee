#include <gui/canvas.h>

#include <cassert>

#include <glm/gtc/matrix_transform.hpp>

#include <QApplication>
#include <QBasicTimer>
#include <QSize>
#include <QDebug>
#include <QResizeEvent>
#include <QExposeEvent>
#include <QEvent>
#include <QPainter>

#include <glbinding/gl/types.h>
#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>
#include <glbinding/gl/bitfield.h>

#include <globjects/globjects.h>
#include <globjects/Error.h>
#include <globjects/DebugMessage.h>

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

#include <gui/CanvasImplementation.h>


Canvas::Canvas(const GLFormat & format, QWidget * parent)
:   QWidget(parent)
,   m_refreshTimeMSec(0)
,   m_painter(nullptr)
,   m_navigationHistory(nullptr)
,   m_eventHandler(nullptr)
,   m_timer(new QBasicTimer)
,   m_format(format)
,   m_canvasImplementation(new CanvasImplementation(format))
,   m_canvasWidget(QWidget::createWindowContainer(m_canvasImplementation, this))
,   m_isInitialized(false)
{
    setMinimumSize(1, 1);
    // Important for overdraw, not occluding the scene.
    setAutoFillBackground(false);

    m_navigationHistory = new NavigationHistory();
    connect(m_canvasImplementation, SIGNAL(renderingRequested()), this, SLOT(onRenderingRequested()));
    m_canvasImplementation->installEventFilter(this);
    m_canvasWidget->installEventFilter(this);
}

Canvas::~Canvas()
{
    delete m_timer;
    //m_qtCanvas, m_qtCanvasWidget should be deleteted by Qt, because Canvas owns them
}

void Canvas::initializeGL()
{
    m_canvasImplementation->makeCurrent();
    
    globjects::init();
    globjects::DebugMessage::enable();

    qDebug("Vendor: %s", qPrintable(GPUQuery::vendor()));
    qDebug("Renderer: %s", qPrintable(GPUQuery::renderer()));
    qDebug("Version: %s", qPrintable(GPUQuery::version()));

    if (!GPUQuery::isCoreProfile())
    {
        qDebug("Memory (total):     %i MiB", GPUQuery::totalMemory() / 1024);
        qDebug("Memory (available): %i MiB\n", GPUQuery::availableMemory() / 1024);
    }
    
    gl::glClearColor(1.f, 1.f, 1.f, 1.f);
    glError();

    m_isInitialized = true;
}

void Canvas::resizeEvent(QResizeEvent *event)
{  
    if (!m_isInitialized)
        initializeGL();
    
    makeCurrent();
    glError();

    if (m_painter)
        m_painter->resize(event->size().width(), event->size().height());

    if (m_eventHandler)
        m_eventHandler->resize(event->size());

    m_canvasWidget->resize(event->size());

    QWidget::resizeEvent(event);

    update();
}

void Canvas::paint()
{
	glError();
    if (!m_isInitialized)
        initializeGL();
	glError();
    m_canvasImplementation->paint();
}

void Canvas::paintGL()
{
    glError();
    if (m_painter)
        m_painter->paint();
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
    
    //// ToDo: Review after painter refactoring
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
    
    if (m_painter)
        m_painter->resize(w, h);
    if (m_eventHandler)
        m_eventHandler->resize(QSize(w, h));

    Projection * tempProjection = camera->getProjection();
    camera->setProjection(projection);
    delete tempProjection;
    camera->setView(view);

    return frame.mirrored(false, true); // flip vertically
}

void Canvas::resize(int width, int height)
{
    QWidget::resize(width, height);
    m_eventHandler->resize(QSize(width, height));
}

AbstractEventHandler * Canvas::eventHandler()
{
    return m_eventHandler;
}

void Canvas::setEventHandler(AbstractEventHandler * eventHandler)
{
	if (eventHandler == m_eventHandler)
	    return;
    if (m_eventHandler != nullptr)
        disconnect(dynamic_cast<AbstractNavigation*>(m_eventHandler), SIGNAL(navigated()), this, SLOT(update()));
	m_eventHandler = eventHandler;
	m_eventHandler->resize(size());

    /// Links navigation and navigation history and the view changed signal.
    m_navigationHistory->setNavigation(dynamic_cast<AbstractNavigation*>(m_eventHandler));
    dynamic_cast<AbstractNavigation*>(m_eventHandler)->viewChanged.connect(this, &Canvas::saveHistory);
    connect(dynamic_cast<AbstractNavigation*>(m_eventHandler), SIGNAL(navigated()), this, SLOT(update()));
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
    update();
}

void Canvas::mouseReleaseEvent(QMouseEvent * event)
{
    if (m_eventHandler)
        m_eventHandler->mouseReleaseEvent(event);

	emit mouseReleaseEventSignal(event);
    update();
}

void Canvas::mouseMoveEvent(QMouseEvent * event)
{
    this->setFocus();
    
    if (m_eventHandler)
        m_eventHandler->mouseMoveEvent(event);

	emit mouseMoveEventTriggered(1);
    update();
}

void Canvas::wheelEvent(QWheelEvent * event)
{
    if (m_eventHandler)
        m_eventHandler->wheelEvent(event);
    update();
}

void Canvas::onRenderingRequested()
{
    paintGL();
}

const GLFormat& Canvas::format()
{
    return m_format;
}

void Canvas::update()
{
    QWidget::update();
    paint();
}

bool Canvas::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::UpdateRequest:
        update();
        return true;
    default:
        return QWidget::event(event);
    }
}

void Canvas::exposeEvent(QExposeEvent *event)
{
    update();
}

bool Canvas::eventFilter(QObject *obj, QEvent *event)
{
    switch (event->type())
    {
    case QEvent::MouseMove:
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease:
    case QEvent::Wheel:
        this->event(event);
        return true;
    default:
        return !(obj == m_canvasImplementation || obj == m_canvasWidget);
    }
}

void Canvas::makeCurrent()
{
    if (!m_isInitialized) initializeGL();
    m_canvasImplementation->makeCurrent();
    globjects::setCurrentContext();
}
