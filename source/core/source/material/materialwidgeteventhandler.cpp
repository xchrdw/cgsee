#include <core/material/materialwidgeteventhandler.h>

#include <QGLWidget>

#include <core/camera.h>

MaterialWidgetEventHandler::MaterialWidgetEventHandler(Camera * camera)
    : m_camera(camera)
    , m_canvas(0)
{
}

MaterialWidgetEventHandler::~MaterialWidgetEventHandler() { }

void MaterialWidgetEventHandler::updateCamera()
{
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

void MaterialWidgetEventHandler::sceneChanged(Group * scene)
{
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
void MaterialWidgetEventHandler::resize(const QSize & size) {}

void MaterialWidgetEventHandler::onViewChanged()
{
	//viewChanged(m_viewmatrix, m_fovy);
}
