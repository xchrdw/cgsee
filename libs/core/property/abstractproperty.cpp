
#include "abstractproperty.h"
#include "propertylist.h"
#include "announcer.h"


AbstractProperty::AbstractProperty(QString name, QString description)
:   m_name(name)
,   m_description(description)
,   m_announcer(new Announcer(this))
{
}

AbstractProperty::~AbstractProperty()
{
    delete m_announcer;
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

void AbstractProperty::subscribe(int event, std::function<void(AbstractProperty &)> functor)
{
    m_announcer->subscribe(event, functor);
}

Announcer & AbstractProperty::announcer() const
{
    return *m_announcer;
}

