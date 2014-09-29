
#include <core/painter/abstractpainter.h>

#include <QKeyEvent>
#include <QPainter>
#include <QImage>

#include <core/camera/abstractcamera.h>
#include <globjects/globjects.h>
#include <core/property/propertylist.h>


AbstractPainter::AbstractPainter()
:   m_initialized(false)
,   m_propertylist(new PropertyList())
{
}
 
AbstractPainter::~AbstractPainter()
{
}

void AbstractPainter::paint()
{
    if(m_initialized)
        return;

    if(!(m_initialized = initialize()))
        qFatal("Painter initialization failed.");
}

void AbstractPainter::resize(
    const int width
,   const int height)
{
    gl::glViewport(0, 0, width, height);


    if(m_initialized)
        return;

    if(!(m_initialized = initialize()))
        qFatal("Painter initialization failed.");
}
