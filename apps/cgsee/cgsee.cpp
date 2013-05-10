#include "cgsee.h"
#include "painter.h"

#include <gui/viewer.h>



CGSee::CGSee(int & argc, char ** argv)
:   AbstractApplication(argc, argv)
,   m_viewer(nullptr)
,   m_painter(nullptr)
{
    // Create Viewer

    m_viewer = new Viewer();
    m_viewer->setWindowTitle(title());
    m_viewer->initialize(format());

    QFileSystemModel *dirModel = new QFileSystemModel;
    dirModel->setRootPath(QDir::currentPath());
    dirModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);

    QTreeView *dirTree = new QTreeView();
    dirTree->setModel(dirModel);

    QDockWidget * leftNav = new QDockWidget(tr("Explorer"));
    leftNav->setWidget(dirTree);
    leftNav->setAllowedAreas(Qt::LeftDockWidgetArea);
    m_viewer->addDockWidget(Qt::LeftDockWidgetArea, leftNav);

    m_painter = new Painter();
    m_viewer->setPainter(m_painter);

    // Start

    m_viewer->show();
}

CGSee::~CGSee()
{
    delete m_painter;
    delete m_viewer;
}
