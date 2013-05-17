#include <GL/glew.h>

#include <cassert>

#include <QOpenGLContext>
#include <QSettings>

#include "ui_viewer.h"

#include "viewer.h"
#include "canvas.h"
#include "core/abstractpainter.h"
#include "core/abstractnavigation.h"
#include "core/flightnavigation.h"
#include "core/fpsnavigation.h"
#include "core/savedviews.h"
#include "core/arcballnavigation.h"
#include "canvasexporter.h"

#include <core/abstractpainter.h>
#include <core/fileassociatedshader.h>
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


void Viewer::on_captureAsImageAction_triggered()
{
    assert(m_qtCanvas);
    CanvasExporter::save(*m_qtCanvas, this);
}

void Viewer::on_captureAsImageAdvancedAction_triggered()
{
    assert(m_qtCanvas);
    CanvasExporter::save(*m_qtCanvas, this, true);
}

void Viewer::on_reloadAllShadersAction_triggered()
{
    FileAssociatedShader::reloadAll();
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

void Viewer::setSavedViews(SavedViews * savedViews)
{
    m_savedViews = savedViews;
    m_savedViews->setCanvas(m_qtCanvas);
}

SavedViews * Viewer::savedViews() {
    return m_savedViews;
}


void Viewer::setCamera(Camera * camera )
{
    m_camera = camera;
}

Camera * Viewer::camera()
{
    return m_camera;
}
    
void Viewer::deactivateManipulators() {
    m_ui->flightManipulatorAction->setChecked(false);
    m_ui->fpsManipulatorAction->setChecked(false);
    m_ui->trackballManipulatorAction->setChecked(false);
}

void Viewer::setFlightManipulator()
{
    setNavigation(new FlightNavigation(m_camera));
    deactivateManipulators();
    m_ui->flightManipulatorAction->setChecked(true);
    qDebug("Flight navigation, use WASD and arrow keys");
}

void Viewer::setTrackballManipulator()
{
    setNavigation(new ArcballNavigation(m_camera));
    deactivateManipulators();
    m_ui->trackballManipulatorAction->setChecked(true);
    qDebug("Arcball navigation, use left and right mousebuttons");
}
    
void Viewer::setFpsManipulator()
{
    setNavigation(new FpsNavigation(m_camera));
    deactivateManipulators();
    m_ui->fpsManipulatorAction->setChecked(true);
    qDebug("FPS Navigation, use mouse and WASD");
}



void Viewer::keyPressEvent(QKeyEvent * event)
{
    if(event->isAutoRepeat()) {
        return;
    }

    switch (event->key())
    {
    case Qt::Key_Escape:
        close();
        break;
    case Qt::Key_1:
        setFlightManipulator();
        break;
    case Qt::Key_2:
        setTrackballManipulator();
        break;    
    case Qt::Key_3:
        setFpsManipulator();
        break;
    case Qt::Key_R:
        navigation()->reset();
        break;

    default:
        if (Qt::Key_F1 <= event->key() && event->key() <= Qt::Key_F12)
        {
            if(event->modifiers() == Qt::ControlModifier) {
                savedViews()->saveView(event->key() - Qt::Key_F1);
            } else {
                savedViews()->loadView(event->key() - Qt::Key_F1);
            }
        }
        else {
            navigation()->keyPressEvent(event);
        }
        break;
    }
}

void Viewer::keyReleaseEvent( QKeyEvent *event )
{
    if(event->isAutoRepeat()) {
        return;
    }
    m_qtCanvas->navigation()->keyReleaseEvent(event);
}

void Viewer::on_flightManipulatorAction_triggered(){
    setFlightManipulator();
}
    
void Viewer::on_trackballManipulatorAction_triggered(){
    setTrackballManipulator();
}
    
void Viewer::on_fpsManipulatorAction_triggered(){
    setFpsManipulator();
}
