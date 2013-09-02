
#include "propertylist.h"

PropertyList::PropertyList()
:   m_properties(new QList<AbstractProperty *>())
,   m_properties_map(new QHash<QString, AbstractProperty *>())
{
}

PropertyList::~PropertyList()
{
    qDeleteAll(*m_properties); 
    delete m_properties;
    delete m_properties_map;
}


/** Public **/

bool PropertyList::isEmpty() const
{
    return m_properties->isEmpty();
}

bool PropertyList::exists(QString name) const
{
    return m_properties_map->value(name, nullptr);
}

bool PropertyList::add(AbstractProperty * property)
{
    if (!exists(property->name())) {
        m_properties->push_back(property);
        m_properties_map->insert(property->name(), property);
        return true;
    } else {
        return false;
    }
}

bool PropertyList::remove(QString name)
{
    if (exists(name)) {
        AbstractProperty * property = this->value(name);
        m_properties->removeAll(property);
        m_properties_map->remove(name);
        delete property;
        return true;  
    } else {
        return false;
    }
}

AbstractProperty * PropertyList::value(QString name) const
{
    AbstractProperty * property = m_properties_map->value(name, nullptr);
    if (!property)
        qFatal("Requested Property \"%s\" not found", qPrintable(name));

    return property;
}

QList<AbstractProperty *> PropertyList::list() const
{
    return *m_properties;
}