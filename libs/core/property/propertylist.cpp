
#include "propertylist.h"

PropertyList::PropertyList()
:   m_properties(nullptr)
,   m_properties_map(nullptr)
{
}

PropertyList::~PropertyList()
{
    // qDeleteAll(*m_properties); 
    // ... leads to "*** error for object 0x1088adb70: pointer being freed was not allocated"
    delete m_properties;
    delete m_properties_map;
}


/** Public **/

bool PropertyList::exists(QString name)
{
    return propertiesMap()->value(name, nullptr);
}

bool PropertyList::add(AbstractProperty * property)
{
    if (!exists(property->name())) {
        properties()->push_back(property);
        propertiesMap()->insert(property->name(), property);
        return true;
    } else {
        return false;
    }
}

bool PropertyList::remove(QString name)
{
    if (exists(name)) {
        AbstractProperty * property = this->value(name);
        properties()->removeAll(property);
        propertiesMap()->remove(name);
        delete property;
        return true;  
    } else {
        return false;
    }
}

AbstractProperty * PropertyList::value(QString name)
{
    AbstractProperty * property = this->propertiesMap()->value(name, nullptr);
    if (!property)
        qFatal("Requested Property \"%s\" not found", qPrintable(name));

    return property;
}

QList<AbstractProperty *> PropertyList::list()
{
    return *(this->properties());
}


/** Protected **/

QList<AbstractProperty *> * PropertyList::properties()
{
    if (!m_properties)
        m_properties = new QList<AbstractProperty *>();
    return m_properties;
}

QHash<QString, AbstractProperty *> * PropertyList::propertiesMap()
{
    if (!m_properties_map)
        m_properties_map = new QHash<QString, AbstractProperty *>();
    return m_properties_map;
}