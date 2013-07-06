
#pragma once

#include "abstractproperty.h"
#include "abstractpropertyvisitor.h"

class AbstractValueProperty : public AbstractProperty
{
    Q_OBJECT
public:
    AbstractValueProperty(QString name, QString description)
    :   AbstractProperty(name, description) {};
signals:
    void changed(AbstractProperty & me);
};

template <typename Type>
class ValueProperty : public AbstractValueProperty
{
public:
    ValueProperty(QString name, QString description, Type value = NULL);
    virtual ~ValueProperty();

    virtual void accept(AbstractPropertyVisitor & visitor);

    virtual Type value() const;
    virtual void setValue(Type value, bool silent = true);

protected:
    Type m_value;
};

template <typename Type>
ValueProperty<Type>::ValueProperty(QString name, QString description, Type value)
:   AbstractValueProperty(name, description)
,   m_value(value)
{
}

template <typename Type>
ValueProperty<Type>::~ValueProperty()
{
}

template <typename Type>
void ValueProperty<Type>::accept(AbstractPropertyVisitor & visitor)
{
    visitor.visitValue(*this);
}

template <typename Type>
Type ValueProperty<Type>::value() const
{
    return m_value;
}

template <typename Type>
void ValueProperty<Type>::setValue(Type value, bool silent)
{
    m_value = value;
    if (!silent)
        emit this->changed(*this);
}