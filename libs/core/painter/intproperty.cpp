#include "intproperty.h"
#include "abstractpropertyvisitor.h"


IntProperty::IntProperty(QString name, QString description, int value)
:   AbstractPainterProperty(name, description)
,   m_value(value)
{
}

IntProperty::~IntProperty()
{
}

void IntProperty::visit(AbstractPropertyVisitor & visitor)
{
    visitor.visitInt(*this);
}

int IntProperty::value() const
{
    return m_value;
}

void IntProperty::setValue(int value)
{
    m_value = value;
}
