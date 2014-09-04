#pragma once

#include <QObject>

#include <core/abstracteventhandler.h>

#include <glm/glm.hpp>

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

	void setViewPort(const int width, const int height);
	glm::mat4 topRightView();
	void setFromMatrix(const glm::mat4 & view);
	float getBBRadius();
	void setBBRadius(float radius);
protected:
    Camera * m_camera;

	int m_width;
	int m_height;

	float m_BBRadius;

	float m_fovy;
	glm::mat4 m_viewmatrix;
	glm::mat4 m_frontView;

private:
    QWidget * m_canvas;
};
