
#pragma once

#include <QString>
#include <QList>
#include <core/declspec.h>

class AbstractPainterProperty;
class BoolProperty;
class AbstractListProperty;
class IntProperty;
class FloatProperty;

template <typename Type>
class GenericProperty;

class CGSEE_API AbstractPropertyVisitor
{
public:
    virtual void visitBool(BoolProperty & boolProperty) = 0;
    virtual void visitList(AbstractListProperty & listProperty) = 0;
    virtual void visitInt(IntProperty & intProperty) = 0;
    virtual void visitFloat(FloatProperty & floatProperty) = 0;
    
    template <typename T> void visitGeneric(T & property);
    virtual void visitGeneric(GenericProperty<int> & intProperty);
    virtual void visitGeneric(GenericProperty<float> & intProperty);

protected:
    void iterateOverProperties(const QList<AbstractPainterProperty *> & properties);
};

template <typename T> 
void AbstractPropertyVisitor::visitGeneric(T & property)
{
    qDebug("Nothing todo for property with name %s", qPrintable(property.name()));
}
