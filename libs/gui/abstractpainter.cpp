
#include <QKeyEvent>

#include "abstractpainter.h"

#include <core/gpuquery.h>


AbstractPainter::AbstractPainter()
:   m_initialized(false)
,   m_camera(nullptr)
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

Camera * AbstractPainter::camera()
{
    return m_camera;
}

