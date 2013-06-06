
#include <QFileSystemModel>
#include <QDockWidget>
#include <QMenu>

#include "fileNavigator.h"
#include "fileExplorer.h"
#include "viewer.h"

#include "navigationHandler.h"

#include <core/group.h>
#include <core/assimploader.h>
#include <core/abstractscenepainter.h>


NavigationHandler::NavigationHandler(
    Viewer * parent)

:   m_viewer(parent)
,   m_dockLeft(new QDockWidget(tr("Navigator")))
,   m_dockBottom(new QDockWidget(tr("Explorer")))
,   m_navigator(new FileNavigator(m_dockLeft))
,   m_explorer(new FileExplorer(m_dockBottom))
,   m_loader(new AssimpLoader())
{
    m_dockLeft->setWidget(m_navigator);
    m_dockBottom->setWidget(m_explorer);

    m_viewer->addDockWidget(Qt::LeftDockWidgetArea, m_dockLeft);
    m_viewer->addDockWidget(Qt::BottomDockWidgetArea, m_dockBottom);

    m_explorer->model()->setNameFilters(m_loader->allLoadableTypes());

        
    QObject::connect(
        m_navigator, SIGNAL(clickedDirectory(const QString &)),
        m_explorer, SLOT(setRoot(const QString &)));

    QObject::connect(
        m_explorer, SIGNAL(activatedItem(const QString &)),
        this, SLOT(loadFile(const QString &)));
}

NavigationHandler::~NavigationHandler()
{
    delete m_dockLeft;
    delete m_dockBottom;
    delete m_loader;
}

void NavigationHandler::loadFile(const QString & path)
{
    Group * scene = m_loader->importFromFile(path);
    m_viewer->painter()->assignScene(scene);
}