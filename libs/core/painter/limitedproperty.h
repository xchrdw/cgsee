
#pragma once

#include "valueproperty.h"

template <typename Type>
class LimitedProperty : public ValueProperty<Type>
{
public:
    LimitedProperty(QString name, QString description, Type value = NULL);
    LimitedProperty(QString name, QString description, Type value, Type min, Type max);
    ~LimitedProperty();

    virtual void setValue(Type value);

    Type minimum() const;
    void setMinimum(Type min);
    Type maximum() const;
    void setMaximum(Type max);

protected:
    Type m_min;
    Type m_max;
};

template <typename Type>
LimitedProperty<Type>::LimitedProperty(QString name, QString description, Type value)
:   ValueProperty(name, description, value)
,   m_min(NULL)
,   m_max(NULL)
{
}

template <typename Type>
LimitedProperty(QString name, QString description, Type value, Type min, Type max)
:   ValueProperty(name, description, value)
,   m_min(min)
,   m_max(max)
{
}

template <typename Type>
void LimitedProperty::setValue(Type value)
{
    if ((m_min <= value) && (value <= m_max))
        m_value = value;
}

template <typename Type>
Type LimitedProperty<Type>::minimum() const
{
    return m_min;
}

template <typename Type>
void LimitedProperty<Type>::setMinimum(Type min)
{
    m_min = min;
}

template <typename Type>
Type LimitedProperty<Type>::maximum() const
{
    return m_max;
}

template <typename Type>
void LimitedProperty<Type>::setMaximum(Type max)
{
    m_max = max;
}


