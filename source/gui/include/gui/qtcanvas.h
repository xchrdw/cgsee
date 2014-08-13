#pragma once

#include <gui/gui_api.h>

#include <QWindow>

class GLFormat;
class QOpenGLContext;
class QResizeEvent;

class GUI_API QtCanvas : public QWindow
{
    Q_OBJECT

public:
    QtCanvas(const GLFormat & format);
    virtual ~QtCanvas();

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