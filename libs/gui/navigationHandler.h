
#pragma once



#include <QObject>

#include <core/declspec.h>


class QMainWindow;
class QDockWidget;

class FileNavigator;
class FileExplorer;
class AbstractModelLoader;


class CGSEE_API NavigationHandler : public QObject
{
    Q_OBJECT

public:
    NavigationHandler(QMainWindow * parent = nullptr);
    virtual ~NavigationHandler();


public slots:
    void triggeredLoadFile(const bool & triggered);
    void loadFile(const QModelIndex & index);

protected:
    QMainWindow * m_viewer;

    QDockWidget * m_dockLeft;
    QDockWidget * m_dockBottom;

    FileNavigator * m_navigator;
    FileExplorer * m_explorer;
    AbstractModelLoader * m_loader;
};