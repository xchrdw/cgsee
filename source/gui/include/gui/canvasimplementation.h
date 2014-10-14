#pragma once

#include <gui/gui_api.h>

#include <QWindow>

class GLFormat;
class QOpenGLContext;
class QResizeEvent;

/**
*   @brief The implementation of a canvas. 
*   @details A QWindow is used as drawing surface. This QWindow is connected with a QOpenGLContext in order to enable drawing with OpenGL.
*   Both are wrapped by this class in order to do not include gl.h.
*/

class GUI_API CanvasImplementation : public QWindow
{
    Q_OBJECT

public:
    CanvasImplementation(const GLFormat & format);
    virtual ~CanvasImplementation();

    void makeCurrent();
    void doneCurrent();

    void swapBuffers();

    void paint();

signals:
    void renderingRequested();

protected:
    bool event(QEvent *event);
    virtual void resizeEvent(QResizeEvent * event) override;
    virtual void exposeEvent(QExposeEvent * event) override;

protected:
    QOpenGLContext *m_context;
};