
#include "abstractproperty.h"
#include "propertylist.h"


AbstractProperty::AbstractProperty(QString name, QString description)
:   m_name(name)
,   m_description(description)
,   m_children(nullptr)
{
}

AbstractProperty::~AbstractProperty()
{
    delete m_children;
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

PropertyList * AbstractProperty::children()
{
    if (!m_children)
        m_children = new PropertyList();
    return m_children;
}
