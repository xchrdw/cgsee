
#pragma once

#ifdef WIN32
#include <windows.h>
#else
#include <GL/glx.h>  // for GLXContext
#endif

#include <memory>

#include <QMainWindow>


#include <core/declspec.h>


class GLFormat;

class Ui_Viewer;
class Canvas;
class AbstractScenePainter;

class FileNavigator;
class FileExplorer;
class Viewer;
class AbstractModelLoader;
class Group;


class CGSEE_API Viewer : public QMainWindow
{
    Q_OBJECT
public:
    Viewer(
        QWidget * parent = nullptr
    ,   Qt::WindowFlags flags = nullptr);
    virtual ~Viewer();

    void initialize(const GLFormat & format);

    void setPainter(AbstractScenePainter * painter);
    AbstractScenePainter * painter();

protected slots:
    void on_captureAsImageAction_triggered();
    void on_captureAsImageAdvancedAction_triggered();

    void on_reloadAllShadersAction_triggered();

    void on_openFileDialogAction_triggered();

    void on_loadFile(const QString & path);

protected:

    void initializeNavigation();
    void initializeDockWidgets(QDockWidget * dockWidget,
        QWidget * widget, Qt::DockWidgetArea area);

#ifdef WIN32
    const HGLRC createQtContext(const GLFormat & format);
    static const HGLRC currentContextHandle();
#elif __APPLE__
    void * createQtContext(const GLFormat & format);
    static void * currentContextHandle();
#else
    const GLXContext createQtContext(const GLFormat & format);
    static const GLXContext currentContextHandle();
#endif

protected:
    const std::unique_ptr<Ui_Viewer> m_ui;

    Canvas * m_qtCanvas;

    QDockWidget * m_dockLeft;
    QDockWidget * m_dockBottom;

    FileNavigator * m_navigator;
    FileExplorer * m_explorer;
    AbstractModelLoader * m_loader;
};
