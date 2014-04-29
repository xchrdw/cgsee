
#pragma once

#include <typeinfo>
#include <QString>
#include <core/declspec.h>
#include "announcer.h"

class AbstractPropertyVisitor;
class PropertyList;

class CGSEE_API AbstractProperty
{
public:
    AbstractProperty(QString name, QString description);
    virtual ~AbstractProperty();

    virtual void accept(AbstractPropertyVisitor & visitor) = 0;

    QString name() const;
    void setName(QString name);
    QString description() const;
    void setDescription(QString name);

    template <class Property>
    Property * to();

    template <typename Object>
    void subscribe(int event, Object * object,
        void (Object::*method_pointer)(AbstractProperty &));

    void subscribe(int event, std::function<void(AbstractProperty &)> functor);

protected:
    Announcer & announcer() const;

    QString m_name;
    QString m_description;
    Announcer * m_announcer;
};

template <class Object>
void AbstractProperty::subscribe(int event, Object * object,
    void (Object::*method_pointer)(AbstractProperty &))
{
    m_announcer->subscribe(event, object, method_pointer);
}

template <class Property>
Property * AbstractProperty::to()
{
    Property * property = dynamic_cast<Property *>(this);
    if (!property)
        qFatal("Requested Property \"%s\" is not of Type \"%s\"", 
            qPrintable(this->name()), typeid(Property).name());
    return property;
}
