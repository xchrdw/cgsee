
#pragma once

#include <functional>
#include <QList>
#include <core/declspec.h>

class AbstractProperty;

class CGSEE_API Announcer
{
public:
    Announcer(AbstractProperty * property);
    virtual ~Announcer();

    template <typename ObjectClass>
    void subscribe(int event, ObjectClass * object,
        void (ObjectClass::*method_pointer)(AbstractProperty &));

    void subscribe(int event, std::function<void(AbstractProperty &)> lambda);
    void notify(int event);

protected:
    QList<std::function<void(AbstractProperty &)>> & subscriptons(int event);

    QList<QList<std::function<void(AbstractProperty &)>> *> * m_subscriptions;
    AbstractProperty * m_property;
};

template <typename ObjectClass>
void Announcer::subscribe(int event, ObjectClass * object,
    void (ObjectClass::*method_pointer)(AbstractProperty &))
{
    this->subscribe(event, [object, method_pointer] (AbstractProperty & property) {
        (object->*method_pointer)(property);
    });
}