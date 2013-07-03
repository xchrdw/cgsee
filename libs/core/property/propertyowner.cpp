
#include "propertyowner.h"

PropertyOwner::PropertyOwner()
:   m_properties(nullptr)
,   m_properties_map(nullptr)
{
}

PropertyOwner::~PropertyOwner()
{
    // qDeleteAll(*m_properties); 
    // ... leads to "*** error for object 0x1088adb70: pointer being freed was not allocated"
    delete m_properties;
    delete m_properties_map;
}


/** Public **/

bool PropertyOwner::propertyExists(QString name)
{
    return propertiesMap()->value(name, nullptr);
}

bool PropertyOwner::addProperty(AbstractProperty * property)
{
    if (!propertyExists(property->name())) {
        properties()->push_back(property);
        propertiesMap()->insert(property->name(), property);
        return true;
    } else {
        return false;
    }
}

bool PropertyOwner::removeProperty(QString name)
{
    if (propertyExists(name)) {
        AbstractProperty * property = property(name);
        properties()->removeAll(property);
        propertiesMap()->remove(name);
        delete property;
        return true;  
    } else {
        return false;
    }
}

AbstractProperty * PropertyOwner::property(QString name)
{
    AbstractProperty * property = this->propertiesMap().value(name, nullptr);
    if (!property)
        qFatal("Requested Property \"%s\" not found", qPrintable(name));

    return property;
}

QList<AbstractProperty *> PropertyOwner::properties() const
{
    return *(this->properties());
}


/** Protected **/

QList<AbstractProperty *> * PropertyOwner::properties()
{
    if (!m_properties)
        m_properties = new QList<AbstractProperty *>();
    return m_properties;
}

QHash<QString, AbstractProperty *> * PropertyOwner::propertiesMap()
{
    if (!m_properties_map)
        m_properties_map = new QHash<QString, AbstractProperty *>();
    return m_properties_map;
}