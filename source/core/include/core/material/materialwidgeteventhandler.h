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
	
	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);
	void mouseMoveEvent(QMouseEvent * event);
	void mousePressEvent(QMouseEvent * event);
	void mouseReleaseEvent(QMouseEvent * event);
	void mouseDoubleClickEvent(QMouseEvent * event);
	void wheelEvent(QWheelEvent * event);
	void resizeEvent(QResizeEvent * event);
	void resize(const QSize & size);
	void MaterialWidgetEventHandler::onViewChanged();

protected:
    Camera * m_camera;

private:
    QWidget * m_canvas;
};
