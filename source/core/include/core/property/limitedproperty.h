#pragma once

#include <core/core_api.h>

#include <core/property/valueproperty.h>

template <typename Type>
class CORE_API LimitedProperty : public ValueProperty<Type>
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
    if (m_min > m_max) {
        std::swap(m_min, m_max);
    }
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
        ValueProperty<Type>::setValue(value);
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
    if (m_min != min && min <= m_max) {
        m_min = min;

        if (m_min > this->m_value)
            this->m_value = m_min;

        this->announcer().notify(kLimitsChanged);
    }
}

template <typename Type>
Type LimitedProperty<Type>::maximum() const
{
    return m_max;
}

template <typename Type>
void LimitedProperty<Type>::setMaximum(Type max)
{
    if (m_max != max && m_min >= max) {
        m_max = max;

        if (m_max < this->m_value)
            this->m_value = m_max;

        this->announcer().notify(kLimitsChanged);
    }
}
