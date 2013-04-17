
#pragma once

#include <core/declspec.h>

#include <QtOpenGL>
#include <QMainWindow>

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

    // Create canvas with a qt context (based on GLWidget).
    const HGLRC createQtContext(const GLFormat & format);
    // Create viewer with a osg context (based on GLWidget).
    const HGLRC createOsgContext(const GLFormat & format);

    static const HGLRC currentContextHandle();

protected:
	const std::unique_ptr<Ui_Viewer> m_ui;

    Canvas * m_qtCanvas;
};