
#pragma once

#include <QStringList>

#include <core/declspec.h>
#include <core/glformat.h>

class QKeyEvent;
class Camera;
class AbstractGLParent;
class AbstractPainterProperty;

class CGSEE_API AbstractPainter
{
public:
    AbstractPainter(void);
    virtual ~AbstractPainter();

    // calls initialize if not initialized - so call this when subclassed
    virtual void paint();

    virtual void setShading(char shader) = 0;

    const QImage capture(
        AbstractGLParent & parent
    ,   const QSize & size
    ,   const bool aspect
    ,   const bool alpha);

    virtual void resize(
        const int width
    ,   const int height);

    const QList<AbstractPainterProperty *> & properties() const;

protected:
    virtual const bool initialize() = 0;
    virtual Camera * camera() = 0;

protected:
    bool m_initialized;
    QList<AbstractPainterProperty *> * m_properties;
};
