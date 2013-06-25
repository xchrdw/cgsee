
#include "boolproperty.h"
#include "abstractpropertyvisitor.h"

BoolProperty::BoolProperty(QString name)
:   AbstractPainterProperty(name)
,   m_enabled(false)
{
}

BoolProperty::BoolProperty(QString name, QString description)
:   AbstractPainterProperty(name, description)
,   m_enabled(false)
{
}

BoolProperty::BoolProperty(QString name, bool enabled)
:   AbstractPainterProperty(name)
,   m_enabled(enabled)
{
}

BoolProperty::BoolProperty(QString name, QString description, bool enabled)
:   AbstractPainterProperty(name, description)
,   m_enabled(enabled)
{
}

BoolProperty::~BoolProperty()
{
    AbstractPainterProperty::~AbstractPainterProperty();
}

void BoolProperty::visit(AbstractPropertyVisitor & visitor)
{
    visitor.visitBool(*this);
}

bool BoolProperty::enabled() const
{
    return m_enabled;
}

void BoolProperty::setEnabled(bool enabled)
{
    m_enabled = enabled;
}

BoolProperty * BoolProperty::toBool()
{
    return this;
}