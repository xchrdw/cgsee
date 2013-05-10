
#pragma once

#include <gui/fileNavigator.h>
#include <gui/fileExplorer.h>

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
class AbstractPainter;
class FileNavigator;
class FileExplorer;


class CGSEE_API Viewer : public QMainWindow
{
public:
    Viewer(
        QWidget * parent = nullptr
    ,   Qt::WindowFlags flags = nullptr);
    virtual ~Viewer();

    void initialize(const GLFormat & format);

    void setPainter(AbstractPainter * painter);
    AbstractPainter * painter();

protected:

    void initializeNavigatorExplorer();

#ifdef WIN32
    // Create canvas with a qt context (based on GLWidget).
    const HGLRC createQtContext(const GLFormat & format);
    static const HGLRC currentContextHandle();
#else
    // Create canvas with a qt context (based on GLWidget).
    const GLXContext createQtContext(const GLFormat & format);
    static const GLXContext currentContextHandle();
#endif

protected:
    const std::unique_ptr<Ui_Viewer> m_ui;

    Canvas * m_qtCanvas;
    FileNavigator * m_fileNavigator;
    FileExplorer * m_fileExplorer;
};
