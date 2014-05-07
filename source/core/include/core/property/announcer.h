#pragma once

#include <core/core_api.h>

#include <functional>

#include <QList>
#include <QHash>


class AbstractProperty;

class CORE_API Announcer
{
public:
    Announcer(AbstractProperty * property);
    virtual ~Announcer();

    template <typename Object>
    void subscribe(int event, Object * object,
        void (Object::*method_pointer)(AbstractProperty &));

    void subscribe(int event, std::function<void(AbstractProperty &)> functor);
    void notify(int event);

protected:
    QList<std::function<void(AbstractProperty &)>> & subscriptions(int event);

    QHash<int, QList<std::function<void(AbstractProperty &)>> *> * m_subscriptions;
    AbstractProperty * m_property;
};

template <typename Object>
void Announcer::subscribe(int event, Object * object,
    void (Object::*method_pointer)(AbstractProperty &))
{
    this->subscribe(event, [object, method_pointer] (AbstractProperty & property) {
        (object->*method_pointer)(property);
    });
}
