
#pragma once

#include <functional>
#include <QList>
#include <QHash>

template <typename EventsEnum>
class Announcer
{
public:
    Announcer();
    virtual ~Announcer();

    template <typename ObjectClass>
    void subscribe(EventsEnum event, ObjectClass * object,
        void (ObjectClass::*method_pointer)());

    void subscribe(EventsEnum event, std::function<void()> lambda);
    void notify(EventsEnum event);

protected:
    QList<std::function<void()>> & subscriptons(EventsEnum event);

    QHash<EventsEnum, QList<std::function<void()>> *> * m_subscriptions;
};

template <typename EventsEnum>
Announcer<EventsEnum>::Announcer()
:   m_subscriptions(new QHash<EventsEnum, QList<std::function<void()>> *>())
{
}

template <typename EventsEnum>
Announcer<EventsEnum>::~Announcer()
{
    qDeleteAll(*m_subscriptions);
    delete m_subscriptions;
}

template <typename EventsEnum>
template <typename ObjectClass>
void Announcer<EventsEnum>::subscribe(EventsEnum event, ObjectClass * object,
    void (ObjectClass::*method_pointer)())
{
    this->subscribe(event, [object, method_pointer] () {
        (object->*method_pointer)();
    });
}

template <typename EventsEnum>
void Announcer<EventsEnum>::subscribe(EventsEnum event, std::function<void()> lambda)
{
    this->subscriptons(event).append(lambda);
}

template <typename EventsEnum>
void Announcer<EventsEnum>::notify(EventsEnum event) {
    for (std::function<void()> & lambda : this->subscriptons(event)) {
        lambda();
    }
}

template <typename EventsEnum>
QList<std::function<void()>> & Announcer<EventsEnum>::subscriptons(EventsEnum event)
{
    if (!m_subscriptions->value(event, nullptr))
        m_subscriptions->insert(event, new QList<std::function<void()>>());

    return *m_subscriptions->value(event);
}