
#include <gui/canvas.h>

#include <GL/glew.h>

#include <cassert>

#include <QApplication>
#include <QBasicTimer>

#include <core/navigation/abstractnavigation.h>
#include <core/navigation/abstractscenenavigation.h>
#include <core/navigation/flightnavigation.h>
#include <core/navigation/arcballnavigation.h>

#include <core/painter/abstractscenepainter.h>
#include <core/gpuquery.h>
#include <core/glformat.h>
#include <core/timer.h>


Canvas::Canvas(
    const GLFormat & format,
    QWidget * parent)

:   QGLWidget(format.asQGLFormat(), parent)
,   m_refreshTimeMSec(0)
,   m_painter(nullptr)
,   m_navigation(nullptr)
,   m_timer(nullptr)
,   m_format(format)
{
    m_timer = new QBasicTimer();
    //m_timer->start(format.vsync() ? 1000/60 : 0, this);

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
    if(m_painter)
        m_painter->resize(width, height);
    if(m_navigation)
        m_navigation->setViewPort(width, height);
    // if(m_coordinateProvider)
    //     m_coordinateProvider->resize(width, height);

}


// http://doc.qt.digia.com/qt/opengl-overpainting-glwidget-cpp.html
// http://harmattan-dev.nokia.com/docs/library/html/qt4/opengl-overpainting.html
// -> does not work for core profile or modern rendering

//void Canvas::paintEvent(QPaintEvent *)
//{
//    glError();
//    paint();
//    glError();
//
//    // The fixed function OGL is used to support old school OpenGL calls for overlay painting.
//    // NOTE: it is not tested, what happens on contexts in newer core profiles.
//
//    // NOTE: QPainter is off use here: http://qt-project.org/forums/viewthread/26510
//    // It does not support 3.2 core profile rendering "any time soon"...
//
//    //QPainter painter(this);
//    //paintOverlay(painter);
//    //painter.end();
//    //glError();
//}

//void Canvas::paintOverlay(QPainter & painter)
//{
//    painter.setRenderHint(QPainter::Antialiasing);
//    painter.setRenderHint(QPainter::TextAntialiasing);
//}

void Canvas::paintGL()
{
    glError();  
    if(m_painter)
        m_painter->paint();
    else 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     
    glError();  
}

void Canvas::timerEvent(QTimerEvent *event)
{
    assert(m_timer);
    if(event->timerId() != m_timer->timerId())
        return;

    update();
}

void Canvas::setRefreshTimeMSec(int msec)
{
    m_refreshTimeMSec = msec;
    if (msec < 0) {
        m_timer->stop();
    } else {
        m_timer->start(m_refreshTimeMSec, this);
    }
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

#include <QSize>

const QImage Canvas::capture(
    const QSize & size
,   const bool aspect
,   const bool alpha)
{
    if(!m_painter)
        return QImage();

    //return m_painter->capture(*this, size, aspect, alpha);
}

void Canvas::resize(int width, int height)
{
    QGLWidget::resize(width, height);
}

AbstractNavigation * Canvas::navigation()
{
    return m_navigation;
}

void Canvas::setNavigation( AbstractNavigation * navigation )
{
    float bbRadius = 0;
    AbstractSceneNavigation *asn = dynamic_cast<AbstractSceneNavigation*>(m_navigation);
    if (asn){
        bbRadius = asn->getBBRadius();
        delete m_navigation;
    }
    m_navigation = navigation;
    m_navigation->setCanvas(this);
    asn = dynamic_cast<AbstractSceneNavigation*>(m_navigation);
    if (asn && bbRadius != 0)
        asn->setBBRadius(bbRadius);
}

void Canvas::mousePressEvent( QMouseEvent * event )
{
    m_navigation->mousePressEvent(event);
}

void Canvas::mouseReleaseEvent( QMouseEvent * event )
{
    m_navigation->mouseReleaseEvent(event);

    emit mouseReleaseEventSignal(event);
}

void Canvas::mouseMoveEvent( QMouseEvent * event )
{
    this->setFocus(); 
    m_navigation->mouseMoveEvent(event);

    emit mouseMoveEventTriggered(1);
}

void Canvas::wheelEvent(QWheelEvent * event)
{
    m_navigation->wheelEvent(event);
}
