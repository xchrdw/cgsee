
#include "boolproperty.h"
#include "abstractpropertyvisitor.h"

BoolProperty::BoolProperty()
:   AbstractPainterProperty()
,   m_enabled(false)
{
}

BoolProperty::BoolProperty(QString name)
:   AbstractPainterProperty(name)
,   m_enabled(false)
{
}

BoolProperty::BoolProperty(bool enabled)
:   AbstractPainterProperty()
,   m_enabled(enabled)
{
}

BoolProperty::BoolProperty(QString name, bool enabled)
:   AbstractPainterProperty(name)
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