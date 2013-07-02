
#include "propertyowner.h"

PropertyOwner::PropertyOwner()
:   m_properties(new QHash<QString, AbstractProperty *>())
{
}

PropertyOwner::~PropertyOwner()
{
    // qDeleteAll(*m_properties); 
    // ... leads to "*** error for object 0x1088adb70: pointer being freed was not allocated"
    delete m_properties;
}

bool PropertyOwner::propertyExists(QString name)
{
    return m_properties->value(name, nullptr);
}

bool PropertyOwner::addProperty(AbstractProperty * property)
{
    if (!this->propertyExists(property->name())) {
        m_properties->insert(property->name(), property);
        return true;
    } else
        return false;
}

bool PropertyOwner::removeProperty(QString name)
{
    delete this->property(name);
    return m_properties->remove(name);
}

AbstractProperty * PropertyOwner::property(QString name)
{
    AbstractProperty * property = m_properties->value(name, nullptr);
    if (!property)
        qFatal("Requested Property \"%s\" not found", qPrintable(name));

    return property;
}

const QList<AbstractProperty *> PropertyOwner::properties() const
{
    return m_properties->values();
}