
#include <GL/glew.h>

#include <QOpenGLContext>
#include <QSettings>

#include "ui_viewer.h"

#include "viewer.h"
#include "canvas.h"
#include "abstractpainter.h"
#include "core/abstractnavigation.h"
#include "core/flightnavigation.h"
#include "core/arcballnavigation.h"
#include <core/glformat.h>


namespace 
{
    const QString SETTINGS_GEOMETRY ("Geometry");
    const QString SETTINGS_STATE    ("State");
}


Viewer::Viewer(
    QWidget  * parent,
    Qt::WindowFlags flags)

:   QMainWindow(parent, flags)
,   m_ui(new Ui_Viewer)

,   m_qtCanvas(nullptr)
{
    m_ui->setupUi(this);
    
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings s;

    restoreGeometry(s.value(SETTINGS_GEOMETRY).toByteArray());
    restoreState(s.value(SETTINGS_STATE).toByteArray());
};

#ifdef WIN32
const HGLRC Viewer::currentContextHandle()
{
    return  wglGetCurrentContext();
#elif __APPLE__
void * Viewer::currentContextHandle()
{
    return nullptr;
#else
const GLXContext Viewer::currentContextHandle()
{
    return glXGetCurrentContext();
#endif
}

#ifdef WIN32
const HGLRC Viewer::createQtContext(const GLFormat & format)
#elif __APPLE__
void * Viewer::createQtContext(const GLFormat & format)
#else
const GLXContext Viewer::createQtContext(const GLFormat & format)
#endif
{
    m_qtCanvas = new Canvas(format, this);
    setCentralWidget(m_qtCanvas);

    QGLContext * qContext(const_cast<QGLContext *>(m_qtCanvas->context()));

    if(!qContext)
        qFatal("Creating QtGL-Context failed.");

    qContext->makeCurrent();

    if(QGLContext::currentContext() != qContext)
        qFatal("Making QtGL-Context current failed.");

#ifdef WIN32
    const HGLRC qtContextHandle = currentContextHandle();
#elif __APPLE__
    void * qtContextHandle = currentContextHandle();
#else
    const GLXContext qtContextHandle = currentContextHandle();
#endif

    // NOTE: might work even if no context was returned. 
    // This just double checks...

    if(nullptr == qtContextHandle)
        qWarning("Acquiring QtGL-Context handle failed.");

    // canvas verifies the context itself.

    qContext->doneCurrent();

    return qtContextHandle;
}

void Viewer::initialize(const GLFormat & format)
{
    if(!QGLFormat::hasOpenGL())
        qFatal("OpenGL not supported.");

    createQtContext(format);
}

Viewer::~Viewer()
{
    QSettings s;
    s.setValue(SETTINGS_GEOMETRY, saveGeometry());
    s.setValue(SETTINGS_STATE, saveState());

    delete m_qtCanvas;
}

void Viewer::setPainter(AbstractPainter * painter)
{
    if(!m_qtCanvas)
        return;

    m_qtCanvas->setPainter(painter);
}

AbstractPainter * Viewer::painter()
{
    if(!m_qtCanvas)
        return nullptr;

    return m_qtCanvas->painter();
}

void Viewer::setNavigation(AbstractNavigation * navigation)
{
    m_qtCanvas->setNavigation(navigation);
}

AbstractNavigation * Viewer::navigation() {
    if(!m_qtCanvas)
        return nullptr;
    return m_qtCanvas->navigation();
}

void Viewer::setCamera(Camera * camera )
{
    m_camera = camera;
}

Camera * Viewer::camera()
{
    return m_camera;
}

void Viewer::keyPressEvent(QKeyEvent * event)
{
    m_qtCanvas->navigation()->keyPressEvent(event);

    switch (event->key())
    {
    case Qt::Key_1:
        setNavigation(new FlightNavigation(m_camera)); break;
    case Qt::Key_2:
        setNavigation(new ArcballNavigation(m_camera)); break;
    default:
        if (event->key() >= Qt::Key_F1 && event->key() <= Qt::Key_F12)
        {
            if(event->modifiers() == Qt::ControlModifier) {
                navigation()->saveView(event->key() - Qt::Key_F1);
            } else {
                navigation()->loadView(event->key() - Qt::Key_F1);
            }
            
        }
        break;
    }



}

void Viewer::keyReleaseEvent( QKeyEvent *event )
{
    m_qtCanvas->navigation()->keyReleaseEvent(event);
}




