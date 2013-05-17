#pragma once

#include <stdio.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "declspec.h"

#include <QObject>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QBasicTimer>

class Camera;
class QWidget;

class CGSEE_API AbstractNavigation : QObject {
    
public:

    AbstractNavigation(Camera *camera);
    virtual ~ AbstractNavigation();

    virtual void reset();

    //void assignCoordinateProvider(CoordinateProvider *provider);

    // interaction mapping
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);

    virtual void mouseMoveEvent(QMouseEvent * event);
    virtual void mousePressEvent(QMouseEvent * event);
    virtual void mouseReleaseEvent(QMouseEvent * event);
    virtual void mouseDoubleClickEvent(QMouseEvent * event);
    virtual void wheelEvent(QWheelEvent *event);

    virtual const glm::mat4 viewMatrix();

    void updateCamera();
    virtual void onCameraChanged();

    void setCanvas(QWidget * canvas);
    void setViewPort(const int width, const int height);

protected:
    virtual void timerEvent(QTimerEvent* event);
    void startTimer();
    void stopTimer();
    bool isTimerRunning();


protected:
    int m_width;
    int m_height;
    glm::mat4 m_viewMatrix;

    static const int TIMER_MS = 1000/60;

private:
    Camera * m_camera;
    QWidget * m_canvas;
    QBasicTimer m_timer;
    int m_timer_requests;
};