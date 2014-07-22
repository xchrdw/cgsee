#pragma once

#include <core/core_api.h>

#include <QStringList>

#include <core/glformat.h>

class QKeyEvent;
class AbstractCamera;
class AbstractResizable;
class PropertyList;

class CORE_API AbstractPainter
{
public:
    AbstractPainter(void);
    virtual ~AbstractPainter();

    // calls initialize if not initialized - so call this when subclassed
    virtual void paint();

    virtual void resize(
        const int width
    ,   const int height);

protected:
    virtual bool initialize() = 0;
    virtual AbstractCamera * camera() = 0;

protected:
    bool m_initialized;
    PropertyList * m_propertylist;
};
