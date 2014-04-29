
#pragma once

#include <QHash>
#include <QList>
#include "abstractproperty.h"

class CGSEE_API PropertyList
{
public:
    PropertyList();
    virtual ~PropertyList();

    bool isEmpty() const;
    bool exists(QString name) const;
    bool add(AbstractProperty * property);
    bool remove(QString name);

    AbstractProperty * value(QString name) const;
    
    template <class Property>
    Property * value(QString name) const;

    QList<AbstractProperty *> list() const;

private:
    QList<AbstractProperty *> * m_properties;
    QHash<QString, AbstractProperty *> * m_properties_map;
};

template <class Property>
Property * PropertyList::value(QString name) const
{
    return this->value(name)->to<Property>();
}