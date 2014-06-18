#include <core/navigation/abstractnavigation.h>

AbstractNavigation::AbstractNavigation()
    : m_width(0)
    , m_height(0)
    , m_canvas(0)
{
}


AbstractNavigation::~AbstractNavigation()
{
}


void AbstractNavigation::keyPressEvent(QKeyEvent *event) { }
void AbstractNavigation::keyReleaseEvent(QKeyEvent *event) { }

void AbstractNavigation::mouseMoveEvent(QMouseEvent * event) { }
void AbstractNavigation::mousePressEvent(QMouseEvent * event) { }
void AbstractNavigation::mouseReleaseEvent(QMouseEvent * event) { }
void AbstractNavigation::mouseDoubleClickEvent(QMouseEvent * event) { }

void AbstractNavigation::wheelEvent(QWheelEvent * event) { }

void AbstractNavigation::setCanvas(QWidget * canvas)
{
    m_canvas = canvas;
}

void AbstractNavigation::setViewPort(const int width, const int height)
{
    m_width = width;
    m_height = height;
}