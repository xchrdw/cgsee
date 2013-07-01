
#pragma once

#include <QString>
#include <QList>
#include <core/declspec.h>

class AbstractPainterProperty;
class AbstractListProperty;

template <typename Type>
class ValueProperty;

class CGSEE_API AbstractPropertyVisitor
{
public:
    template <typename Type> 
    void visitGeneric(ValueProperty<Type> & property);

    virtual void visitList(AbstractListProperty & listProperty) = 0;
    virtual void visitGeneric(ValueProperty<bool> & property) = 0;
    virtual void visitGeneric(ValueProperty<float> & property) = 0;
    virtual void visitGeneric(ValueProperty<int> & property) = 0;

protected:
    void iterateOverProperties(const QList<AbstractPainterProperty *> & properties);
};

template <typename Type>
void AbstractPropertyVisitor::visitGeneric(ValueProperty<Type> & property)
{
    qWarning("No specialized visit method found for property: \"%s\"", qPrintable(property.name()));
}
