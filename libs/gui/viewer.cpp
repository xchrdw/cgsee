#include <GL/glew.h>

#include <cassert>

#include <QOpenGLContext>
#include <QSettings>

#include "ui_viewer.h"

#include "viewer.h"
#include "canvas.h"
//#include "core/camera.h"
#include "core/abstractpainter.h"
#include "core/abstractnavigation.h"
#include "core/flightnavigation.h"
#include "core/fpsnavigation.h"
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
,   m_saved_views(12)
,   m_qtCanvas(nullptr)
{
    m_ui->setupUi(this);
    
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings s;

    restoreGeometry(s.value(SETTINGS_GEOMETRY).toByteArray());
    restoreState(s.value(SETTINGS_STATE).toByteArray());

    // TODO: load from settings
    m_saved_views[0] = glm::lookAt(glm::vec3(0.f, 0.f,-2.f), glm::vec3(0), glm::vec3(0.f, 1.f, 0.f));
    m_saved_views[1] = glm::lookAt(glm::vec3(2.f, 0.f, 0.f), glm::vec3(0), glm::vec3(0.f, 1.f, 0.f));
    m_saved_views[2] = glm::lookAt(glm::vec3(0.f, 0.f, 2.f), glm::vec3(0), glm::vec3(0.f, 1.f, 0.f));
    m_saved_views[3] = glm::lookAt(glm::vec3(-2.f, 0.f, 0.f), glm::vec3(0), glm::vec3(0.f, 1.f, 0.f));
    m_saved_views[4] = glm::lookAt(glm::vec3(0.f, 2.f, 0.f), glm::vec3(0), glm::vec3(0.f, 1.f, 0.f));
    m_saved_views[5] = glm::lookAt(glm::vec3(0.f, -2.f, 0.f), glm::vec3(0), glm::vec3(0.f, 1.f, 0.f));
    for (int i = 6; i < m_saved_views.size(); i++)
    {
        m_saved_views[i] = m_saved_views[0];
    }
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
    qDebug("Arcball navigation, use left and right mouse buttons");
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
    case Qt::Key_R:
        navigation()->reset();
        break;

    default:
        if (Qt::Key_F6 <= event->key() && event->key() <= Qt::Key_F12)
        {
            int num = event->key() - Qt::Key_F1;
            if(event->modifiers() == Qt::ControlModifier) {
                //m_saved_views[num] = camera()->view(); needs to have #include "core/camera.h fixed
            } else {
                navigation()->loadView(m_saved_views[num]);
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
    if (Qt::Key_F1 <= event->key() && event->key() <= Qt::Key_F12) {
        // change animations
    } else {
        m_qtCanvas->navigation()->keyReleaseEvent(event);
    }
}

void Viewer::on_flightManipulatorAction_triggered() {
    setFlightManipulator();
}
    
void Viewer::on_trackballManipulatorAction_triggered() {
    setTrackballManipulator();
}
    
void Viewer::on_fpsManipulatorAction_triggered() {
    setFpsManipulator();
}



void Viewer::on_actionFrontView_triggered() {
    navigation()->loadView(m_saved_views[0]);
}
void Viewer::on_actionLeftView_triggered() {
    navigation()->loadView(m_saved_views[1]);
}
void Viewer::on_actionBackView_triggered() {
    navigation()->loadView(m_saved_views[2]);
}
void Viewer::on_actionRightView_triggered() {
    navigation()->loadView(m_saved_views[3]);
}
void Viewer::on_actionTopView_triggered() {
    navigation()->loadView(m_saved_views[4]);
}
void Viewer::on_actionBottomView_triggered() {
    navigation()->loadView(m_saved_views[5]);
}


void Viewer::on_actionLoad_1_triggered() {
    navigation()->loadView(m_saved_views[6]);
}

void Viewer::on_actionLoad_2_triggered() {
    navigation()->loadView(m_saved_views[7]);
}

void Viewer::on_actionLoad_3_triggered() {
    navigation()->loadView(m_saved_views[8]);
}

void Viewer::on_actionLoad_4_triggered() {
    navigation()->loadView(m_saved_views[9]);
}


void Viewer::on_actionSave_1_triggered() {
    //m_saved_views[6] = camera->view(); needs #include "core/camera.h" fixed
}
void Viewer::on_actionSave_2_triggered() {
    //m_saved_views[7] = camera->view();
}
void Viewer::on_actionSave_3_triggered() {
    //m_saved_views[8] = camera->view();
}
void Viewer::on_actionSave_4_triggered() {
    //m_saved_views[9] = camera->view();
}
