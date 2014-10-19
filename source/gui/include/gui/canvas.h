#pragma once

#include <gui/gui_api.h>

#include <QWidget>

#include <core/glformat.h>

#include <core/navigation/navigationhistory.h>

class QBasicTimer;
class QEvent;
class QTimerEvent;
class QResizeEvent;
class QExposeEvent;
class Timer;

class AbstractPainter;
class AbstractEventHandler;
class AbstractCamera;
class CanvasImplementation;

/**
*   @brief A canvas where can be drawn on with OpenGL.
*   @detail The Canvas encapsulates an OpenGL context. If OpenGL commands are called, it have to be ensured, that the right canvas is current.
*/

class GUI_API Canvas : public QWidget
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

    virtual const GLFormat& format();

    virtual void setRefreshTimeMSec(int msec = 1);
    int refreshTimeMSec() const;

    void makeCurrent();
    void paint();

signals:
	void mouseReleaseEventSignal(QMouseEvent * event);
	void mouseMoveEventTriggered(int triggered);

public slots:
    void onRenderingRequested();
    void update();

protected:
    // For Rendering Loop
    void timerEvent(QTimerEvent *event);

    void resizeEvent(QResizeEvent *event);
    void exposeEvent(QExposeEvent *event);
    bool event(QEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

    void initializeGL();
    virtual void paintGL();

protected:
    NavigationHistory * m_navigationHistory;
    AbstractPainter * m_painter;
    AbstractEventHandler * m_eventHandler;
    CanvasImplementation * m_canvasImplementation;
    QWidget * m_canvasWidget;

    int m_refreshTimeMSec;

    QBasicTimer * m_timer;
    float m_lastEvent;

    const GLFormat m_format;
    bool m_isInitialized;
};
