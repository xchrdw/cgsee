
#include <core/navigation/imagenavigation.h>

#include <core/painter/imagepainter.h>

ImageNavigation::ImageNavigation()
    : m_mouse_last(0)
    , m_mouse_cur(0)
    , m_zoom_on(false)
    , m_panning_on(false)
{
}

ImageNavigation::~ImageNavigation()
{
}

void ImageNavigation::updatePanning()
{
    glm::vec2 delta = (m_mouse_cur - m_mouse_last) / (float)m_height; // normalized height difference
    m_painter->setPanDelta(glm::vec2(-delta.x,delta.y));
}

void ImageNavigation::updateZoom()
{
    float delta = (m_mouse_cur - m_mouse_last).y / m_height; // normalized height difference
    m_painter->setZoomDelta(delta);
}


void ImageNavigation::mouseMoveEvent(QMouseEvent * event)
{
    m_mouse_cur = glm::vec2(event->pos().x(), event->pos().y());
    if (m_mouse_cur != m_mouse_last) { // avoid div/0
        if (m_panning_on) {
            updatePanning();
        } else if (m_zoom_on) {
            updateZoom();
        }
        m_mouse_last = m_mouse_cur;
    }
}

void ImageNavigation::mousePressEvent(QMouseEvent * event)
{
    m_panning_on = event->button() == Qt::RightButton;
    m_zoom_on = event->button() == Qt::MiddleButton;
    m_mouse_last = m_mouse_cur = glm::vec2(event->pos().x(), event->pos().y());
}

void ImageNavigation::mouseReleaseEvent(QMouseEvent * event)
{
    if (event->button() == Qt::RightButton) {
        m_panning_on = false;
    } else if (event->button() == Qt::MiddleButton) {
        m_zoom_on = false;
    }
}

void ImageNavigation::wheelEvent(QWheelEvent *event){
    float delta = event->delta()*0.001;
    m_painter->setZoomDelta(delta);
}

void ImageNavigation::setPainter(ImagePainter * painter) {
    m_painter = painter;
}