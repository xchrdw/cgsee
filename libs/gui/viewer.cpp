#include <QFileSystemModel>
#include <QTreeView>
#include <QListView>
#include <QDockWidget>

#include <QOpenGLContext>
#include <QSettings>

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
{
    m_ui->setupUi(this);

    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings s;

    restoreGeometry(s.value(SETTINGS_GEOMETRY).toByteArray());
    restoreState(s.value(SETTINGS_STATE).toByteArray());

    createFileExplorer();
};

void Viewer::createFileExplorer()
{
    QString currentPath = QDir::currentPath();

    QFileSystemModel *dirModel = new QFileSystemModel;
    dirModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    // dirModel->setRootPath(currentPath);

    QTreeView *treeView = new QTreeView;
    treeView->setModel(dirModel);
    treeView->setRootIndex(dirModel->setRootPath(currentPath));

    QFileSystemModel *fileModel = new QFileSystemModel;
    fileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
    // fileModel->setRootPath(currentPath);

    QListView *listView = new QListView;
    listView->setModel(fileModel);
    listView->setRootIndex(fileModel->setRootPath(currentPath));

    QDockWidget * leftNav = new QDockWidget(tr("Navigator"));
    leftNav->setWidget(treeView);
    leftNav->setAllowedAreas(Qt::LeftDockWidgetArea);
    this->addDockWidget(Qt::LeftDockWidgetArea, leftNav);

    QDockWidget * bottomExp = new QDockWidget(tr("Explorer"));
    bottomExp->setWidget(listView);
    bottomExp->setAllowedAreas(Qt::BottomDockWidgetArea);
    this->addDockWidget(Qt::BottomDockWidgetArea, bottomExp);
}

#ifdef WIN32
const HGLRC Viewer::currentContextHandle()
{
    return  wglGetCurrentContext();
#else
const GLXContext Viewer::currentContextHandle()
{
    return glXGetCurrentContext();
#endif
}

#ifdef WIN32
const HGLRC Viewer::createQtContext(const GLFormat & format)
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
#else
    const GLXContext qtContextHandle = currentContextHandle();
#endif

    if(nullptr == qtContextHandle)
        qFatal("Acquiring QtGL-Context handle failed.");

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