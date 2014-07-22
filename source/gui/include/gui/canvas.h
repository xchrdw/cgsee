#pragma once

#include <gui/gui_api.h>

#include <GL/glew.h>
#include <QGLWidget>

#include <core/glformat.h>

#include <core/navigation/navigationhistory.h>

class QBasicTimer;
class QTimerEvent;
class Timer;

class AbstractPainter;
class AbstractEventHandler;
class AbstractCamera;

class GUI_API Canvas : public QGLWidget
{
    Q_OBJECT

public:
    Canvas(
        const GLFormat & format,
        QWidget * parent = nullptr);
    virtual ~Canvas();

    NavigationHistory * navigationHistory();
    void setNavigationHistory(AbstractEventHandler * navigation);
    void saveHistory(glm::mat4 viewmatrix, float fovy);
    void setCamera(AbstractCamera * camera);
    AbstractCamera * camera();
    void setPainter(AbstractPainter * painter);
    AbstractPainter * painter();

    void setEventHandler(AbstractEventHandler * eventHandler);
    AbstractEventHandler * eventHandler();

    virtual void mouseMoveEvent(QMouseEvent * event);
    virtual void mousePressEvent(QMouseEvent * event);
    virtual void mouseReleaseEvent(QMouseEvent * event);
    virtual void wheelEvent(QWheelEvent *event);

    const QImage capture(
        const bool alpha = false);
    const QImage capture(
        const QSize & size
    ,   const bool aspect = true
    ,   const bool alpha = false);

    virtual void resize(int width, int height);

    virtual void setRefreshTimeMSec(int msec = 1);
    int refreshTimeMSec() const;

signals:
	void mouseReleaseEventSignal(QMouseEvent * event);
	void mouseMoveEventTriggered(int triggered);

protected:

    // QGLWidget Interface
    virtual void initializeGL();
    virtual void resizeGL(
        int width
    ,   int height);

    // http://doc.qt.digia.com/qt/opengl-overpainting.html
    // "Instead of implementing paintGL() to handle updates to the
    //  widget, we implement a normal QWidget::paintEvent(). This
    //  allows us to mix OpenGL calls and QPainter operations in a
    //  controlled way."

    //virtual void paintEvent(QPaintEvent * event); // Does not work for 3.2 anyway. :(
    //void paintOverlay(QPainter & painter);

    virtual void paintGL();

    // For Rendering Loop
    void timerEvent(QTimerEvent *event);

protected:
    NavigationHistory * m_navigationHistory;
    AbstractPainter * m_painter;
    AbstractCamera * m_camera;
    AbstractEventHandler * m_eventHandler;

    int m_refreshTimeMSec;

    QBasicTimer * m_timer;
    float m_lastEvent;

    const GLFormat m_format;
};
