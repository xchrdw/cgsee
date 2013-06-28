#include "floatproperty.h"
#include "abstractpropertyvisitor.h"


FloatProperty::FloatProperty(QString name, QString description, double value)
:   AbstractPainterProperty(name, description)
,   m_value(value)
{
}

FloatProperty::~FloatProperty()
{
}

void FloatProperty::visit(AbstractPropertyVisitor & visitor)
{
    visitor.visitFloat(*this);
}

double FloatProperty::value() const
{
    return m_value;
}

void FloatProperty::setValue(double value)
{
    m_value = value;
}
