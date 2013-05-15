// #include <QFileSystemModel>
// #include <QTreeView>
// #include <QListView>
// #include <QDockWidget>

#include <QOpenGLContext>
#include <QSettings>
#include <QDockWidget>

#include "ui_viewer.h"

#include "viewer.h"
#include "canvas.h"
#include "abstractpainter.h"

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
,   m_dockLeft(nullptr)
,   m_dockBottom(nullptr)
,   m_fileNavigator(nullptr)
,   m_fileExplorer(nullptr)
{
    m_ui->setupUi(this);

    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings s;

    restoreGeometry(s.value(SETTINGS_GEOMETRY).toByteArray());
    restoreState(s.value(SETTINGS_STATE).toByteArray());

    initializeNavigatorExplorer();
};

void Viewer::initializeNavigatorExplorer()
{
    m_dockLeft = new QDockWidget(tr("Navigator"));
    m_fileNavigator = new FileNavigator(m_dockLeft);
    m_dockLeft->setWidget(m_fileNavigator);

    m_dockBottom = new QDockWidget(tr("Explorer"));
    m_fileExplorer = new FileExplorer(m_dockBottom);
    m_dockBottom->setWidget(m_fileExplorer);

    m_fileNavigator->setExplorer(m_fileExplorer);
    m_fileExplorer->setNavigator(m_fileNavigator);

    this->addDockWidget(Qt::LeftDockWidgetArea, m_dockLeft);
    this->addDockWidget(Qt::BottomDockWidgetArea, m_dockBottom);

    QObject::connect(
        m_fileNavigator, SIGNAL(clicked(const QModelIndex)),
        m_fileExplorer, SLOT(callSetRoot(const QModelIndex)));
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
    delete m_fileNavigator;
    delete m_fileExplorer;
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