
#include "advancedlistproperty.h"
#include "propertylist.h"
#include "abstractpropertyvisitor.h"

AdvancedListProperty::AdvancedListProperty(QString name, QString description)
:   ListProperty(name, description)
,   m_propertylists(new QList<PropertyList *>())
,   m_empty_propertylist(new PropertyList())
{
}

AdvancedListProperty::AdvancedListProperty(QString name, QString description, QStringList choices)
:   ListProperty(name, description, choices)
,   m_propertylists(new QList<PropertyList *>())
,   m_empty_propertylist(new PropertyList())
{
    for (QString choice : choices)
        m_propertylists->append(m_empty_propertylist);
}

AdvancedListProperty::~AdvancedListProperty()
{
    qDeleteAll(*m_propertylists);
    delete m_propertylists;
    delete m_empty_propertylist;
}

void AdvancedListProperty::accept(AbstractPropertyVisitor & visitor)
{
    visitor.visitAdvancedList(*this);
}

PropertyList * AdvancedListProperty::selectedPropertyList() const
{
    return m_propertylists->at(m_selection);
}

bool AdvancedListProperty::add(QString choice)
{
    return add(choice, m_empty_propertylist);
}

bool AdvancedListProperty::add(QString choice, PropertyList * propertylist)
{
    if (ListProperty::add(choice)) {
        m_propertylists->append(propertylist);
        return true;
    } else {
        return false;
    }
}

bool AdvancedListProperty::addList(QStringList choices)
{
    if (ListProperty::addList(choices)) {
        for (QString choice : choices)
            m_propertylists->append(m_empty_propertylist);
        return true;
    } else {
        return false;
    }
}

bool AdvancedListProperty::remove(QString choice)
{
    int index = m_choices.indexOf(choice);
    if (index != kNoIndex) {
        return remove(index);
    } else {
        return false;
    }
}

bool AdvancedListProperty::remove(int index)
{
    if (ListProperty::remove(index)) {
        m_propertylists->removeAt(index);
        return true;
    } else {
        return false;
    }
}

QList<PropertyList *> AdvancedListProperty::propertyLists() const
{
    return *m_propertylists;
}
