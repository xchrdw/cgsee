
#pragma once

#include <QStringList>
#include <QHash>

#include <core/declspec.h>
#include <core/glformat.h>

class QKeyEvent;
class Camera;
class AbstractGLParent;
class AbstractProperty;

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

    bool propertyExists(QString name);
    bool addProperty(AbstractProperty * property);
    bool removeProperty(QString name);

    AbstractProperty * property(QString name);
    
    template <class PainterProperty>
    PainterProperty * property(QString name);


    const QList<AbstractProperty *> properties() const;

protected:
    virtual const bool initialize() = 0;
    virtual Camera * camera() = 0;

protected:
    bool m_initialized;
    QHash<QString, AbstractProperty *> * m_properties;
};

template <class PainterProperty>
PainterProperty * AbstractPainter::property(QString name)
{
    PainterProperty * property = dynamic_cast<PainterProperty *>(this->property(name));
    if (!property)
        qFatal("Requested Property \"%s\" is not of desired Type", qPrintable(name));

    return property;
}
