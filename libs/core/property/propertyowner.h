
#pragma once

#include <QHash>
#include <QList>
#include "abstractproperty.h"

class PropertyOwner
{
public:
    PropertyOwner();
    virtual ~PropertyOwner();

    bool propertyExists(QString name);
    bool addProperty(AbstractProperty * property);
    bool removeProperty(QString name);

    AbstractProperty * property(QString name);
    const QList<AbstractProperty *> properties() const;
    
    template <class PainterProperty>
    PainterProperty * property(QString name);

protected:
    QList<AbstractProperty *> * properties();
    QHash<QString, AbstractProperty *> * propertiesMap();

private:
    QList<AbstractProperty *> * m_properties;
    QHash<QString, AbstractProperty *> * m_properties_map;

};

template <class PainterProperty>
PainterProperty * PropertyOwner::property(QString name)
{
    return this->property(name)->to<PainterProperty>();
}