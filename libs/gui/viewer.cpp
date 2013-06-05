
#include <GL/glew.h>

#include <cassert>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <QOpenGLContext>
#include <QSettings>
#include <QTextStream>

#include "ui_viewer.h"
#include "viewer.h"
#include "canvas.h"
#include "canvasexporter.h"

#include <core/abstractnavigation.h>
#include <core/flightnavigation.h>
#include <core/fpsnavigation.h>
#include <core/arcballnavigation.h>

#include <core/abstractpainter.h>
#include <core/fileassociatedshader.h>
#include <core/glformat.h>


namespace 
{
    const QString SETTINGS_GEOMETRY ("Geometry");
    const QString SETTINGS_STATE    ("State");
    const QString SETTINGS_SAVED_VIEWS    ("SavedViews");
}


Viewer::Viewer(
    QWidget  * parent,
    Qt::WindowFlags flags)

:   QMainWindow(parent, flags)
,   m_ui(new Ui_Viewer)
,   m_qtCanvas(nullptr)
,   m_saved_views(4)
{
    m_ui->setupUi(this);
    
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings s;

    restoreGeometry(s.value(SETTINGS_GEOMETRY).toByteArray());
    restoreState(s.value(SETTINGS_STATE).toByteArray());

    restoreViews(s);


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

void Viewer::keyPressEvent(QKeyEvent * event)
{
    if(!event->isAutoRepeat()) {
        navigation()->keyPressEvent(event);
    }
}

void Viewer::keyReleaseEvent( QKeyEvent *event )
{
    if(!event->isAutoRepeat()) {
        navigation()->keyReleaseEvent(event);
    }
}

void Viewer::on_flightManipulatorAction_triggered() {
    setNavigation(new FlightNavigation(m_camera));
    uncheckManipulatorActions();
    m_ui->flightManipulatorAction->setChecked(true);
    qDebug("Flight navigation, use WASD and arrow keys");
}
    
void Viewer::on_trackballManipulatorAction_triggered() {
    setNavigation(new ArcballNavigation(m_camera));
    uncheckManipulatorActions();
    m_ui->trackballManipulatorAction->setChecked(true);
    qDebug("Arcball navigation, use left and right mouse buttons");
}
    
void Viewer::on_fpsManipulatorAction_triggered() {
    setNavigation(new FpsNavigation(m_camera));
    uncheckManipulatorActions();
    m_ui->fpsManipulatorAction->setChecked(true);
    qDebug("FPS Navigation, use mouse and WASD");
}

void Viewer::uncheckManipulatorActions() {
    m_ui->flightManipulatorAction->setChecked(false);
    m_ui->fpsManipulatorAction->setChecked(false);
    m_ui->trackballManipulatorAction->setChecked(false);
}


// helper to restore mat4
glm::mat4 string2mat(QString s) {
    glm::mat4 mat;
    QStringList list = s.split(';');
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            mat[j][i] = list.at(j + 4*i).toFloat();
        }
    }
    return mat;
}

// helper to save mat4
QString mat2string(glm::mat4 mat) {
    QString s("");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            s += QString::number(mat[j][i]);
            s += ';';
        }
    }
    return s;
}

void Viewer::restoreViews( QSettings &s ) {
    s.beginGroup(SETTINGS_SAVED_VIEWS);
    glm::mat4 default_view = glm::lookAt(glm::vec3(0.f, 0.f, -2.f), glm::vec3(0), glm::vec3(0.f, 1.f, 0.f));
    for (int i = 0; i < m_saved_views.size(); i++)
    {
        QString name = "view_" + QString::number(i+1);
        QString str = s.value(name, "").value<QString>();
        if (str.size() > 0) {
            m_saved_views[i] = string2mat(str);
        } else {
            m_saved_views[i] = default_view;
        }

    }
    s.endGroup();
}

void Viewer::saveView(int i) {
    m_saved_views[i] = navigation()->viewMatrix();
    QSettings s;
    s.beginGroup(SETTINGS_SAVED_VIEWS);
    s.setValue("view_" + QString::number(i+1), mat2string(navigation()->viewMatrix()));
    s.endGroup();
}

void Viewer::loadView(int i) {
    navigation()->loadView(m_saved_views[i]);
}


void Viewer::on_actionFrontView_triggered() {
    navigation()->loadView(glm::lookAt(glm::vec3(0.f, 0.f,-2.f), glm::vec3(0), glm::vec3(0.f, 1.f, 0.f)));
}
void Viewer::on_actionLeftView_triggered() {
    navigation()->loadView(glm::lookAt(glm::vec3(2.f, 0.f, 0.f), glm::vec3(0), glm::vec3(0.f, 1.f, 0.f)));
}
void Viewer::on_actionBackView_triggered() {
    navigation()->loadView(glm::lookAt(glm::vec3(0.f, 0.f, 2.f), glm::vec3(0), glm::vec3(0.f, 1.f, 0.f)));
}
void Viewer::on_actionRightView_triggered() {
    navigation()->loadView(glm::lookAt(glm::vec3(-2.f, 0.f, 0.f), glm::vec3(0), glm::vec3(0.f, 1.f, 0.f)));
}
void Viewer::on_actionTopView_triggered() {
    navigation()->loadView(glm::lookAt(glm::vec3(0.f, 2.f, 0.f), glm::vec3(0), glm::vec3(0.f, 0.f, 1.f)));
}
void Viewer::on_actionBottomView_triggered() {
    navigation()->loadView(glm::lookAt(glm::vec3(0.f, -2.f, 0.f), glm::vec3(0), glm::vec3(0.f, 0.f, -1.f)));
}
void Viewer::on_actionTopRightView_triggered() {
    navigation()->loadView(glm::lookAt(glm::vec3(1.15f, 1.15f, -1.15f), glm::vec3(0), glm::vec3(0.f, 1.f, 0.f)));
}


void Viewer::on_actionLoad_1_triggered() { loadView(0); }
void Viewer::on_actionLoad_2_triggered() { loadView(1); }
void Viewer::on_actionLoad_3_triggered() { loadView(2); }
void Viewer::on_actionLoad_4_triggered() { loadView(3); }

void Viewer::on_actionSave_1_triggered() { saveView(0); }
void Viewer::on_actionSave_2_triggered() { saveView(1); }
void Viewer::on_actionSave_3_triggered() { saveView(2); }
void Viewer::on_actionSave_4_triggered() { saveView(3); }

