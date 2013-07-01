
#pragma once

#include "abstractpainterproperty.h"
#include "abstractpropertyvisitor.h"

template <typename Type>
class GenericProperty : public AbstractPainterProperty
{
public:
    GenericProperty(QString name, QString description, Type value = NULL);
    virtual ~GenericProperty();

    virtual void visit(AbstractPropertyVisitor & visitor);

    Type value() const;
    void setValue(Type value);

protected:
    Type m_value;
};

template <typename Type>
GenericProperty<Type>::GenericProperty(QString name, QString description, Type value)
:   AbstractPainterProperty(name, description)
,   m_value(value)
{
}

template <typename Type>
GenericProperty<Type>::~GenericProperty()
{
}

template <typename Type>
void GenericProperty<Type>::visit(AbstractPropertyVisitor & visitor)
{
    visitor.visitGeneric(*this);
}

template <typename Type>
Type GenericProperty<Type>::value() const
{
    return m_value;
}

template <typename Type>
void GenericProperty<Type>::setValue(Type value)
{
    m_value = value;
}