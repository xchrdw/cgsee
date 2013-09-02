
#include "announcer.h"
#include "abstractproperty.h"

Announcer::Announcer(AbstractProperty * property)
:   m_subscriptions(new QHash<int, QList<std::function<void(AbstractProperty &)>> *>())
,   m_property(property)
{
}

Announcer::~Announcer()
{
    qDeleteAll(*m_subscriptions);
    delete m_subscriptions;
}

void Announcer::subscribe(int event, std::function<void(AbstractProperty &)> functor)
{
    this->subscriptions(event).append(functor);
}

void Announcer::notify(int event)
{
    for (std::function<void(AbstractProperty &)> functor : this->subscriptions(event)) {
        functor(*m_property);
    }
}

QList<std::function<void(AbstractProperty &)>> & Announcer::subscriptions(int event)
{
    if (!m_subscriptions->value(event, nullptr))
        m_subscriptions->insert(event, new QList<std::function<void(AbstractProperty &)>>());
    
    return *m_subscriptions->value(event);
}