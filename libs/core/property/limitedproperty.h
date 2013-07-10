
#pragma once

#include "valueproperty.h"

template <typename Type>
class CGSEE_API LimitedProperty : public ValueProperty<Type>
{
public:
    typedef enum { kValueChanged, kLimitsChanged } Events;

    LimitedProperty(QString name, QString description, Type value = NULL);
    LimitedProperty(QString name, QString description, Type value, Type min, Type max);
    ~LimitedProperty();

    virtual void accept(AbstractPropertyVisitor & visitor);

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
:   ValueProperty<Type>(name, description, value)
,   m_min(NULL)
,   m_max(NULL)
{
}

template <typename Type>
LimitedProperty<Type>::LimitedProperty(QString name, QString description, Type value, Type min, Type max)
:   ValueProperty<Type>(name, description, value)
,   m_min(min)
,   m_max(max)
{
}

template <typename Type>
LimitedProperty<Type>::~LimitedProperty()
{
}

template <typename Type>
void LimitedProperty<Type>::accept(AbstractPropertyVisitor & visitor)
{
    visitor.visitLimited(*this);
}

template <typename Type>
void LimitedProperty<Type>::setValue(Type value)
{
    if ((m_min <= value) && (value <= m_max)) {
        this->m_value = value;
    }
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
    this->announcer().notify(kLimitsChanged);
}

template <typename Type>
Type LimitedProperty<Type>::maximum() const
{
    return m_max;
    this->announcer().notify(kLimitsChanged);
}

template <typename Type>
void LimitedProperty<Type>::setMaximum(Type max)
{
    m_max = max;
}


