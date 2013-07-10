
#include "abstractproperty.h"
#include "propertylist.h"


AbstractProperty::AbstractProperty(QString name, QString description)
:   m_name(name)
,   m_description(description)
{
}

AbstractProperty::~AbstractProperty()
{
}

QString AbstractProperty::name() const
{
    return m_name;
}

void AbstractProperty::setName(QString name)
{
    m_name = name;
}

QString AbstractProperty::description() const
{
    return m_description;
}

void AbstractProperty::setDescription(QString description)
{
    m_description = description;
}
