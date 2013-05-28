
#include <QMainWindow>
#include <QFileSystemModel>
#include <QDockWidget>
#include <QMenu>

#include "fileNavigator.h"
#include "fileExplorer.h"

#include "navigationHandler.h"

#include <core/group.h>
#include <core/assimploader.h>


NavigationHandler::NavigationHandler(
    QMainWindow * parent)

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

    m_explorer->menu()->addAction(QString("Open"), this, SLOT(triggeredLoadFile(const bool)));

    QObject::connect(
        m_navigator, SIGNAL(clicked(const QModelIndex)),
        m_explorer, SLOT(callSetRoot(const QModelIndex)));

    QObject::connect(
        m_explorer, SIGNAL(activated(const QModelIndex)),
        this, SLOT(loadFile(const QModelIndex)));
}

NavigationHandler::~NavigationHandler()
{
    delete m_dockLeft;
    delete m_dockBottom;
    delete m_loader;
}


void NavigationHandler::triggeredLoadFile(const bool & triggered)
{
    this->loadFile(m_explorer->clickedFile());
}

#include <iostream> // just for debug output
void NavigationHandler::loadFile(const QModelIndex & index)
{
    QString filePath = m_explorer->model()->fileInfo(index).absoluteFilePath();
    // std::cout << filePath.toStdString() << std::endl;
    Group * importGroup = m_loader->importFromFile(filePath);
}