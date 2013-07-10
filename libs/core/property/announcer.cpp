
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

void Announcer::subscribe(int event, std::function<void(AbstractProperty &)> lambda)
{
    this->subscriptions(event).append(lambda);
}

void Announcer::notify(int event)
{
    for (std::function<void(AbstractProperty &)> lambda : this->subscriptions(event)) {
        lambda(*m_property);
    }
}

QList<std::function<void(AbstractProperty &)>> & Announcer::subscriptions(int event)
{
    if (!m_subscriptions->value(event, nullptr))
        m_subscriptions->insert(event, new QList<std::function<void(AbstractProperty &)>>());
    
    auto sub = m_subscriptions->value(event);
    return *sub;
}