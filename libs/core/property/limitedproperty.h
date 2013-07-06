
#pragma once

#include "valueproperty.h"

template <typename Type>
class LimitedProperty : public ValueProperty<Type>
{
public:
    LimitedProperty(QString name, QString description, Type value = NULL);
    LimitedProperty(QString name, QString description, Type value, Type min, Type max);
    ~LimitedProperty();

    virtual void accept(AbstractPropertyVisitor & visitor);

    virtual void setValue(Type value, bool silent = true);

    Type minimum() const;
    void setMinimum(Type min, bool silent = true);
    Type maximum() const;
    void setMaximum(Type max, bool silent = true);

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
void LimitedProperty<Type>::setValue(Type value, bool silent)
{
    if ((m_min <= value) && (value <= m_max)) {
        this->m_value = value;
        if (!silent)
            emit this->changed(*this);
    }
}

template <typename Type>
Type LimitedProperty<Type>::minimum() const
{
    return m_min;
}

template <typename Type>
void LimitedProperty<Type>::setMinimum(Type min, bool silent)
{
    m_min = min;
    if (!silent)
        emit this->changed(*this);
}

template <typename Type>
Type LimitedProperty<Type>::maximum() const
{
    return m_max;
}

template <typename Type>
void LimitedProperty<Type>::setMaximum(Type max, bool silent)
{
    m_max = max;
    if (!silent)
        emit this->changed(*this);
}


