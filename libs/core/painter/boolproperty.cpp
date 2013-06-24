
#include "boolproperty.h"

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