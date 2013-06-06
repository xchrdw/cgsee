
#pragma once



#include <QObject>

#include <core/declspec.h>


class QDockWidget;

class FileNavigator;
class FileExplorer;
class Viewer;
class AbstractModelLoader;
class Group;


class CGSEE_API NavigationHandler : public QObject
{
    Q_OBJECT

public:
    NavigationHandler(Viewer * parent = nullptr);
    virtual ~NavigationHandler();


public slots:
    void loadFile(const QString & path);

protected:

    Viewer * m_viewer;

    QDockWidget * m_dockLeft;
    QDockWidget * m_dockBottom;

    FileNavigator * m_navigator;
    FileExplorer * m_explorer;
    AbstractModelLoader * m_loader;
};