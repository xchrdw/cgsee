#include <core/material/materialwidgeteventhandler.h>

#include <QGLWidget>
#include <QMouseEvent>

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

void MaterialWidgetEventHandler::updateArcball()
{
	glm::vec3 va = getArcballVector(m_mouse_last);
	glm::vec3 vb = getArcballVector(m_mouse_cur);

	float angle = glm::acos(std::min(1.0f, glm::dot(va, vb)));
	glm::vec3 axis_in_camera_coord = glm::cross(va, vb);

	glm::mat3 camera2world = glm::mat3(glm::inverse(m_viewmatrix));
	glm::vec3 axis_in_world_coord = camera2world * axis_in_camera_coord;
	m_viewmatrix = m_viewmatrix * glm::rotate(glm::degrees(angle), axis_in_world_coord);
}

/**
* Get a normalized vector from the center of the virtual ball O to a
* point P on the virtual ball surface, such that P is aligned on
* screen's (X,Y) coordinates.  If (X,Y) is too far away from the
* sphere, return the nearest point on the virtual ball surface.
* source: http://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Arcball
*/
glm::vec3 MaterialWidgetEventHandler::getArcballVector(glm::vec2 v)
{
	// normalize mouse coordinates to [-1:1]
	glm::vec3 P = glm::vec3(v.x / m_width * 2.0 - 1.0,
		v.y / m_height * 2.0 - 1.0,
		0);
	P.y = -P.y;
	float P_squared = P.x * P.x + P.y * P.y;
	if (P_squared <= 1 * 1) {                     // P is inside ball
		P.z = sqrt(1 * 1 - P_squared);            // calculate height in ball
	}
	else {
		P = glm::normalize(P);                  // nearest point to the ball
	}
	return P;
}

void MaterialWidgetEventHandler::mousePressEvent(QMouseEvent * event)
{
	m_arcball_on = event->button() == Qt::LeftButton;
	m_mouse_last = m_mouse_cur = glm::vec2(event->pos().x(), event->pos().y());
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

