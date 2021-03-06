#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <QObject>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QBasicTimer>
#include <QTimer>

#include <signalzeug/Signal.h>

#include <core/core_api.h>

#include <core/abstracteventhandler.h>

class Camera;
class QWidget;
class Group;

class CORE_API AbstractNavigation : public QObject, public AbstractEventHandler
{    
public:
    AbstractNavigation(Camera *camera);
    virtual ~ AbstractNavigation();

    virtual void reset();

    // ADD to implement scene dependent navigation
    // void assignCoordinateProvider(CoordinateProvider *provider);

    // interaction mapping
    virtual void keyPressEvent(QKeyEvent * event);
    virtual void keyReleaseEvent(QKeyEvent * event);

    virtual void mouseMoveEvent(QMouseEvent * event);
    virtual void mousePressEvent(QMouseEvent * event);
    virtual void mouseReleaseEvent(QMouseEvent * event);
    virtual void mouseDoubleClickEvent(QMouseEvent * event);
    virtual void wheelEvent(QWheelEvent *event);

    virtual void resizeEvent(QResizeEvent * event);
    virtual void resize(const QSize & size);

    virtual const glm::mat4 & viewMatrix();
    void loadView(const glm::mat4 & viewmatrix, const float fovy = 0, bool save_history = true);
    void onViewChanged();

    void setCanvas(QWidget * canvas);
    void setViewPort(const int width, const int height);

    void rescaleScene(Group * scene);
    void sceneChanged(Group * scene);
    void setCamera(Camera * camera);

    glm::mat4 defaultView();
    glm::mat4 frontview();
    glm::mat4 rightview();
    glm::mat4 backview();
    glm::mat4 leftview();
    glm::mat4 topview();
    glm::mat4 bottomview();
    glm::mat4 topRightView();
    glm::mat4 bottomLeftView();
    glm::mat4 randomView();

    glm::mat4 sceneTransform();

    float getBBRadius();
    void setBBRadius(float radius);

    void onNavigated();
    signalzeug::Signal<glm::mat4, float> viewChanged;

protected:
    void startTimer();
    void stopTimer();
    bool isTimerRunning();
    virtual void onTimerEvent();

    virtual void setFromMatrix(const glm::mat4 & view);

    void updateCamera();
    virtual void onCameraChanged(); // override to get notified for camera changes

protected:
    int m_width;
    int m_height;

    float m_BBRadius;

    float m_fovy;
    glm::mat4 m_viewmatrix;

    Camera * m_camera;
    static const float TIMER_MS;
    static const float DURATION;

private:
    void timerEvent(QTimerEvent * event);

    void finishTransition();
    void updateTransition();
    QWidget * m_canvas;
    QBasicTimer m_timer;
    int m_timer_requests;

    QBasicTimer m_eventTimer;

    float m_animation_progress;
    bool m_animation_active;
    glm::vec3 m_old_pos;
    glm::vec3 m_new_pos;
    glm::quat m_old_rotation;
    glm::quat m_new_rotation;
    float m_old_fovy;
    float m_new_fovy;
    glm::mat4 m_frontView;
    glm::mat4 m_sceneTransform;
};
