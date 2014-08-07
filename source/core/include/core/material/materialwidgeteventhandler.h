#pragma once

#include <QObject>

#include <core/abstracteventhandler.h>

class Camera;
class QWidget;
class Group;

class CORE_API MaterialWidgetEventHandler : public QObject, public AbstractEventHandler
{
public:
    MaterialWidgetEventHandler(Camera *camera);
    virtual ~MaterialWidgetEventHandler();

    void setCanvas(QWidget * canvas);
    void sceneChanged(Group * scene);
    void setCamera(Camera * camera);

protected:
    void updateCamera();

protected:
    Camera * m_camera;

private:
    QWidget * m_canvas;
};
