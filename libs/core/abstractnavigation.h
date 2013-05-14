#pragma once

#include <stdio.h>
#include <stdlib.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "declspec.h"

#include <QObject>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QBasicTimer>


class Camera;

class CGSEE_API AbstractNavigation : QObject {
    
    // Q_OBJECT how does this work?

public:

    AbstractNavigation(Camera *camera);
    virtual ~ AbstractNavigation();

    virtual void reset();

    //void assignCoordinateProvider(CoordinateProvider *provider);

    virtual void update(); // called on every frame.


    // interaction mapping

    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);

    virtual void mouseMoveEvent(QMouseEvent * event);
    virtual void mousePressEvent(QMouseEvent * event);
    virtual void mouseReleaseEvent(QMouseEvent * event);
    virtual void mouseDoubleClickEvent(QMouseEvent * event);

    virtual void wheelEvent(QWheelEvent *event);

    virtual const glm::mat4 viewMatrix();
    
    void saveView(int viewnr);
    void loadView(int viewnr);

    void setViewPort(const int width, const int height);

    void updateCamera();

protected:
    int m_width;
    int m_height;
    glm::mat4 m_viewMatrix;
    
    std::vector<glm::mat4> m_saved_views;

private:
    Camera * m_camera;
    void timerEvent(QTimerEvent* event);
    QBasicTimer * m_timer;
    glm::mat4 m_oldMatrix;
    float m_animation;
};