
#include <QKeyEvent>

#include "abstractpainter.h"

#include <core/gpuquery.h>


AbstractPainter::AbstractPainter()
:   m_initialized(false)
{
}
 
AbstractPainter::~AbstractPainter()
{
}

void AbstractPainter::paint()
{
    if(m_initialized)
        return;

    if(m_initialized = initialize())
        qFatal("Painter initialization failed.");
}

void AbstractPainter::resize(
    const int width
,   const int height)
{
    if(m_initialized)
        return;

    if(!(m_initialized = initialize()))
        qFatal("Painter initialization failed.");
}

void AbstractPainter::keyPress(QKeyEvent * event)
{
}

void AbstractPainter::keyRelease(QKeyEvent * event)
{
}

void AbstractPainter::mousePressEvent(QMouseEvent * event)
{
    m_navigation->mousePressEvent(event);
}

void AbstractPainter::mouseReleaseEvent(QMouseEvent * event)
{
    m_navigation->mouseReleaseEvent(event);
}

void AbstractPainter::mouseMoveEvent(QMouseEvent * event)
{
    m_navigation->mouseMoveEvent(event);
}
