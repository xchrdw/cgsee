
#pragma once

#include <functional>
#include <QList>

class Announcer
{
public:
    Announcer();
    virtual ~Announcer();

    template <typename ObjectClass>
    void subscribe(ObjectClass * object,
        void (ObjectClass::*method_pointer)(void));
    void subscribe(std::function<void()> lambda);
    void notify();

protected:
    QList<std::function<void()>> * m_lambdas;
};

Announcer::Announcer()
:   m_lambdas(new QList<std::function<void()>>())
{
}

Announcer::~Announcer()
{
    delete m_lambdas;
}

template <typename ObjectClass>
void Announcer::subscribe(ObjectClass * object,
    void (ObjectClass::*method_pointer) (void))
{
    m_lambdas->append([object, method_pointer] () {
        (object->*method_pointer)();
    });
}

void Announcer::subscribe(std::function<void()> lambda)
{
    m_lambdas->append(lambda);
}

void Announcer::notify() {
    for (std::function<void()> & lambda : *m_lambdas) {
        lambda();
    }
}