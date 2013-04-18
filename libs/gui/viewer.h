
#pragma once

#include <QMainWindow>

#include <core/declspec.h>

#ifdef WIN32
#include <windows.h>
#else
#include <GL/glx.h>  // for GLXContext
#endif

#include <memory>


class GLFormat;

class Ui_Viewer;
class Canvas;


class CGSEE_API Viewer : public QMainWindow
{
public:
    Viewer(
        QWidget * parent = nullptr,
        Qt::WindowFlags flags = NULL);

    virtual ~Viewer();

    void initialize(const GLFormat & format);

protected:

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
};
