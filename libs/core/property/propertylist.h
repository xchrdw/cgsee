
#pragma once

#include <QHash>
#include <QList>
#include "abstractproperty.h"

class PropertyList
{
public:
    PropertyList();
    virtual ~PropertyList();

    bool exists(QString name);
    bool add(AbstractProperty * property);
    bool remove(QString name);

    AbstractProperty * value(QString name);
    QList<AbstractProperty *> list();
    
    template <class PainterProperty>
    PainterProperty * value(QString name);

protected:
    QList<AbstractProperty *> * properties();
    QHash<QString, AbstractProperty *> * propertiesMap();

private:
    QList<AbstractProperty *> * m_properties;
    QHash<QString, AbstractProperty *> * m_properties_map;
};

template <class PainterProperty>
PainterProperty * PropertyList::value(QString name)
{
    return this->value(name)->to<PainterProperty>();
}