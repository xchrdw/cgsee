#pragma once

#include <stdio.h>
#include <stdlib.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "declspec.h"

#include <QApplication>
#include <QFileInfo>
#include <QStringList>
#include <QSettings>
#include <QKeyEvent>

class Camera;

class CGSEE_API AbstractNavigation {

public:

    AbstractNavigation(Camera *camera);
    virtual ~ AbstractNavigation();

    virtual void reset();

    //void assignCoordinateProvider(AbstractCoordinateProvider *provider);

    // interaction mapping

    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);

    virtual void mouseMoveEvent(QMouseEvent * event);
    virtual void mousePressEvent(QMouseEvent * event);
    virtual void mouseReleaseEvent(QMouseEvent * event);
    virtual void mouseDoubleClickEvent(QMouseEvent * event);

    virtual void wheelEvent(QWheelEvent *event);

    virtual const glm::mat4 viewMatrix() = 0;
    //virtual const glm::mat4 viewMatrixInverted() = 0;
    
    void setViewPort( const int width, const int height );
    
    virtual void setFovy(float fovy);
    virtual float getFovy();

protected:
    int m_width;
    int m_height;
    Camera * m_camera;
};