#include <core/material/materialwidgeteventhandler.h>

#include <QGLWidget>

#include <core/camera.h>

#include <glm/gtx/transform.hpp>

MaterialWidgetEventHandler::MaterialWidgetEventHandler(Camera * camera)
    : m_camera(camera)
    , m_canvas(0)
	, m_width(camera->viewport().x)
	, m_height(camera->viewport().y)
	, m_BBRadius(0)
	, m_fovy(camera->fovy())
	, m_viewmatrix(camera->view())
	, m_mouse_last(0)
	, m_mouse_cur(0)
	, m_arcball_on(false)
{
	m_frontView = glm::lookAt(glm::vec3(0.f, 0.f, 2.f), glm::vec3(0), glm::vec3(0.f, 1.f, 0.f));
}

MaterialWidgetEventHandler::~MaterialWidgetEventHandler() { }

void MaterialWidgetEventHandler::updateCamera()
{
	m_camera->setFovy(m_fovy);
	m_camera->setView(m_viewmatrix);
    if (m_canvas)
        m_canvas->repaint();
}

void MaterialWidgetEventHandler::setCanvas(QWidget * canvas)
{
    m_canvas = canvas;
}

void MaterialWidgetEventHandler::setCamera(Camera * camera)
{
    m_camera = camera;
    updateCamera();
}

void MaterialWidgetEventHandler::keyPressEvent(QKeyEvent *event) {}

void MaterialWidgetEventHandler::keyReleaseEvent(QKeyEvent *event) {}

void MaterialWidgetEventHandler::mouseMoveEvent(QMouseEvent * event) {}
void MaterialWidgetEventHandler::mousePressEvent(QMouseEvent * event) {}
void MaterialWidgetEventHandler::mouseReleaseEvent(QMouseEvent * event) {}
void MaterialWidgetEventHandler::mouseDoubleClickEvent(QMouseEvent * event) {}
void MaterialWidgetEventHandler::wheelEvent(QWheelEvent * event) {}
void MaterialWidgetEventHandler::resizeEvent(QResizeEvent * event) {}

void MaterialWidgetEventHandler::resize(const QSize & size) 
{
	setViewPort(size.width(), size.height());
}

void MaterialWidgetEventHandler::setViewPort(const int width, const int height)
{
	m_width = width;
	m_height = height;
}

void MaterialWidgetEventHandler::onViewChanged()
{
	//viewChanged(m_viewmatrix, m_fovy);
}

glm::mat4 MaterialWidgetEventHandler::topRightView()
{
	return m_frontView * glm::rotate(30.f, glm::vec3(1, 0, 0)) * glm::rotate(45.f, glm::vec3(0, 1, 0));
}

void MaterialWidgetEventHandler::setFromMatrix(const glm::mat4 & view)
{
	m_viewmatrix = view;
}

void MaterialWidgetEventHandler::sceneChanged(Group * scene)
{
	AxisAlignedBoundingBox bb = scene->boundingBox();

	m_BBRadius = bb.radius();

	m_frontView = glm::lookAt(bb.center() + glm::vec3(0.f, 0.f, bb.radius()*2.5), bb.center(), glm::vec3(0.f, 1.f, 0.f));
	setFromMatrix(topRightView());
	updateCamera();
}

float MaterialWidgetEventHandler::getBBRadius()
{
	return m_BBRadius;
}

void MaterialWidgetEventHandler::setBBRadius(float radius)
{
	m_BBRadius = radius;
}

