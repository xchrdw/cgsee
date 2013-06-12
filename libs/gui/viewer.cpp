#include <GL/glew.h>

#include <cassert>

#include <QOpenGLContext>
#include <QSettings>
#include <QFileDialog>
#include <QMessageBox>
#include <QDockWidget>
#include <QMenu>

#include "ui_viewer.h"

#include "viewer.h"
#include "canvas.h"
#include "canvasexporter.h"
#include "fileNavigator.h"
#include "fileExplorer.h"

#include <core/abstractscenepainter.h>
#include <core/fileassociatedshader.h>
#include <core/glformat.h>
#include <core/assimploader.h>


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

,   m_dockLeft(new QDockWidget(tr("Navigator")))
,   m_dockBottom(new QDockWidget(tr("Explorer")))
,   m_navigator(new FileNavigator(m_dockLeft))
,   m_explorer(new FileExplorer(m_dockBottom))
,   m_loader(new AssimpLoader())
{
    m_ui->setupUi(this);

    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings s;

    restoreGeometry(s.value(SETTINGS_GEOMETRY).toByteArray());
    restoreState(s.value(SETTINGS_STATE).toByteArray());

    initializeNavigation();
};

void Viewer::initializeNavigation()
{
    this->initializeDockWidgets(m_dockLeft, m_navigator, Qt::LeftDockWidgetArea);
    this->initializeDockWidgets(m_dockBottom, m_explorer, Qt::BottomDockWidgetArea);

    m_explorer->setAllLoadableTypes(m_loader->allLoadableTypes());
        
    QObject::connect(
        m_navigator, SIGNAL(clickedDirectory(const QString &)),
        m_explorer, SLOT(setRoot(const QString &)));

    QObject::connect(
        m_explorer, SIGNAL(activatedItem(const QString &)),
        this, SLOT(on_loadFile(const QString &)));

    QObject::connect(
        m_ui->openFileDialogAction, SIGNAL(changed()),
        this, SLOT(on_openFileDialogAction_triggered()));
}

void Viewer::initializeDockWidgets(QDockWidget * dockWidget, QWidget * widget, Qt::DockWidgetArea area)
{
    dockWidget->setWidget(widget);
    this->addDockWidget(area, dockWidget);
    
#ifdef __APPLE__
    /** 
     THIS IS A BUG WORKAROUND
     The bug lies somewhere in Canvas::Canvas().
     When called in Viewer::createQtContext(), the widgets get messed up.
    **/
    static int count = 0;
    dockWidget->setFloating(true);
    dockWidget->setAllowedAreas(Qt::NoDockWidgetArea);
    
    dockWidget->move(this->pos() - QPoint(dockWidget->width()+5, count-- * (dockWidget->height()+20)));
#endif
}

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

    delete m_dockLeft;
    delete m_dockBottom;
    delete m_loader;
}

void Viewer::setPainter(AbstractScenePainter * painter)
{
    if(!m_qtCanvas)
        return;

    m_qtCanvas->setPainter(painter);
}

AbstractScenePainter * Viewer::painter()
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

void Viewer::on_openFileDialogAction_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath(), m_loader->loadableTypes().join(";;"));
    if (fileName.isEmpty())
        return;
    
    on_loadFile(fileName);
}

void Viewer::on_loadFile(const QString & path)
{
    Group * scene = m_loader->importFromFile(path);
    if (!scene)
        QMessageBox::critical(this, "Loading failed", "The loader was not able to load from \n" + path);
    else {
        this->painter()->assignScene(scene);
        this->m_qtCanvas->update();
    }
}