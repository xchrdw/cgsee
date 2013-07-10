
#pragma once

#include <functional>
#include <QList>
#include <QHash>

template <typename EventsEnum, typename PropertyClass>
class Announcer
{
public:
    Announcer(PropertyClass * property);
    virtual ~Announcer();

    template <typename ObjectClass>
    void subscribe(EventsEnum event, ObjectClass * object,
        void (ObjectClass::*method_pointer)(PropertyClass &));

    void subscribe(EventsEnum event, std::function<void(PropertyClass &)> lambda);
    void notify(EventsEnum event);

protected:
    QList<std::function<void(PropertyClass &)>> & subscriptons(EventsEnum event);

    QHash<EventsEnum, QList<std::function<void(PropertyClass &)>> *> * m_subscriptions;
    PropertyClass * m_property;
};

template <typename EventsEnum, typename PropertyClass>
Announcer<EventsEnum, PropertyClass>::Announcer(PropertyClass * property)
:   m_subscriptions(new QHash<EventsEnum, QList<std::function<void(PropertyClass &)>> *>())
,   m_property(property)
{
}

template <typename EventsEnum, typename PropertyClass>
Announcer<EventsEnum, PropertyClass>::~Announcer()
{
    qDeleteAll(*m_subscriptions);
    delete m_subscriptions;
}

template <typename EventsEnum, typename PropertyClass>
template <typename ObjectClass>
void Announcer<EventsEnum, PropertyClass>::subscribe(EventsEnum event, ObjectClass * object,
    void (ObjectClass::*method_pointer)(PropertyClass &))
{
    this->subscribe(event, [object, method_pointer] (PropertyClass & property) {
        (object->*method_pointer)(property);
    });
}

template <typename EventsEnum, typename PropertyClass>
void Announcer<EventsEnum, PropertyClass>::subscribe(EventsEnum event, std::function<void(PropertyClass &)> lambda)
{
    this->subscriptons(event).append(lambda);
}

template <typename EventsEnum, typename PropertyClass>
void Announcer<EventsEnum, PropertyClass>::notify(EventsEnum event) {
    for (std::function<void(PropertyClass &)> & lambda : this->subscriptons(event)) {
        lambda(*m_property);
    }
}

template <typename EventsEnum, typename PropertyClass>
QList<std::function<void(PropertyClass &)>> & Announcer<EventsEnum, PropertyClass>::subscriptons(EventsEnum event)
{
    if (!m_subscriptions->value(event, nullptr))
        m_subscriptions->insert(event, new QList<std::function<void(PropertyClass &)>>());

    return *m_subscriptions->value(event);
}