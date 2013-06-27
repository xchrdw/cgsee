#include "abstractpainterproperty.h"


AbstractPainterProperty::AbstractPainterProperty(QString name, QString description)
:   m_name(name)
,   m_description(description)
{
}

AbstractPainterProperty::~AbstractPainterProperty()
{
}

QString AbstractPainterProperty::name() const
{
    return m_name;
}

void AbstractPainterProperty::setName(QString name)
{
    m_name = name;
}

QString AbstractPainterProperty::description() const
{
    return m_description;
}

void AbstractPainterProperty::setDescription(QString description)
{
    m_description = description;
}
