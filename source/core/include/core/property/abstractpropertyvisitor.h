#pragma once

#include <core/core_api.h>

#include <QString>
#include <QList>

class QColor;

class AbstractProperty;
class ListProperty;
class AdvancedListProperty;

template <typename Type>
class ValueProperty;
template <typename Type>
class LimitedProperty;

class CORE_API AbstractPropertyVisitor
{
public:
    template <typename Type> 
    void visitValue(ValueProperty<Type> & property);
    template <typename Type> 
    void visitLimited(LimitedProperty<Type> & property);

    virtual void visitList(ListProperty & property) = 0;
    virtual void visitAdvancedList(AdvancedListProperty & property) = 0;
    virtual void visitValue(ValueProperty<bool> & property) = 0;
    virtual void visitValue(ValueProperty<float> & property) = 0;
    virtual void visitValue(ValueProperty<int> & property) = 0;
    virtual void visitValue(ValueProperty<QString> & property) = 0;
    virtual void visitValue(ValueProperty<QColor> & property) = 0;
    virtual void visitLimited(LimitedProperty<int> & property) = 0;
    virtual void visitLimited(LimitedProperty<float> & property) = 0;

protected:
    void iterateOverProperties(const QList<AbstractProperty *> & properties);
};

template <typename Type>
void AbstractPropertyVisitor::visitValue(ValueProperty<Type> & property)
{
    qWarning("No specialized visit method found for ValueProperty: \"%s\"", qPrintable(property.name()));
}

template <typename Type> 
void AbstractPropertyVisitor::visitLimited(LimitedProperty<Type> & property)
{
    qWarning("No specialized visit method found for LimitedProperty: \"%s\"", qPrintable(property.name()));
}
