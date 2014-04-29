
#pragma once

#include "listproperty.h"

class PropertyList;

class CGSEE_API AdvancedListProperty : public ListProperty
{
public:
    AdvancedListProperty(QString name, QString description);
    AdvancedListProperty(QString name, QString description, QStringList choices);
    virtual ~AdvancedListProperty();

    virtual void accept(AbstractPropertyVisitor & visitor);

    PropertyList * selectedPropertyList() const;

    QList<PropertyList *> propertyLists() const;

    virtual bool add(QString choice);
    virtual bool add(QString choice, PropertyList * propertylist);
    virtual bool addList(QStringList choices);
    virtual bool remove(QString choice);
    virtual bool remove(int index);

protected:
    QList<PropertyList *> * m_propertylists;
    PropertyList * m_empty_propertylist;
};