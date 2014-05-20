#pragma once

#include <core/core_api.h>

#include <core/property/abstractproperty.h>
#include <core/property/abstractpropertyvisitor.h>

template <typename Type>
class CORE_API ValueProperty : public AbstractProperty
{
public:
    typedef enum { kValueChanged } Events;

    ValueProperty(QString name, QString description);
    ValueProperty(QString name, QString description, Type value);
    virtual ~ValueProperty();

    virtual void accept(AbstractPropertyVisitor & visitor);

    virtual Type value() const;
    virtual void setValue(Type value);

protected:
    Type m_value;
};

template <typename Type>
ValueProperty<Type>::ValueProperty(QString name, QString description)
:   AbstractProperty(name, description)
{
}

template <typename Type>
ValueProperty<Type>::ValueProperty(QString name, QString description, Type value)
:   AbstractProperty(name, description)
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
void ValueProperty<Type>::setValue(Type value)
{
    if (m_value != value) {
        m_value = value;
        this->announcer().notify(kValueChanged);
    }
}
