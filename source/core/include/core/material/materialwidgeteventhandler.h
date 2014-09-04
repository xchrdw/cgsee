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
	
	virtual void keyPressEvent(QKeyEvent *event) override;
	virtual void keyReleaseEvent(QKeyEvent *event) override;
	virtual void mouseMoveEvent(QMouseEvent * event) override;
	virtual void mousePressEvent(QMouseEvent * event) override;
	virtual void mouseReleaseEvent(QMouseEvent * event) override;
	virtual void mouseDoubleClickEvent(QMouseEvent * event) override;
	virtual void wheelEvent(QWheelEvent * event) override;
	virtual void resizeEvent(QResizeEvent * event) override;
	virtual void resize(const QSize & size) override;

protected:
    Camera * m_camera;

private:
    QWidget * m_canvas;
};
