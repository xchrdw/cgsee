
#include <QMainWindow>
#include <QDockWidget>

#include "fileNavigator.h"
#include "fileExplorer.h"

#include "navigationHandler.h"


NavigationHandler::NavigationHandler(
    QMainWindow * parent)

:   m_viewer(parent)
,   m_dockLeft(new QDockWidget(tr("Navigator")))
,   m_dockBottom(new QDockWidget(tr("Explorer")))
,   m_navigator(new FileNavigator(m_dockLeft))
,   m_explorer(new FileExplorer(m_dockBottom))
{
    m_dockLeft->setWidget(m_navigator);
    m_dockBottom->setWidget(m_explorer);

    m_viewer->addDockWidget(Qt::LeftDockWidgetArea, m_dockLeft);
    m_viewer->addDockWidget(Qt::BottomDockWidgetArea, m_dockBottom);

    QObject::connect(
        m_navigator, SIGNAL(clicked(const QModelIndex)),
        m_explorer, SLOT(callSetRoot(const QModelIndex)));
}

NavigationHandler::~NavigationHandler()
{
    delete m_dockLeft;
    delete m_dockBottom;
}