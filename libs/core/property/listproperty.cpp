
#include "listproperty.h"

#include "abstractpropertyvisitor.h"
#include "propertylist.h"

ListProperty::ListProperty(QString name, QString description)
:   AbstractProperty(name, description)
,   m_selection(kNoIndex)
,   m_propertylists(new QList<PropertyList *>())
,   m_empty_propertylist(new PropertyList())
{
}

ListProperty::ListProperty(QString name, QString description, QStringList choices)
:   AbstractProperty(name, description)
,   m_choices(choices)
,   m_selection(kNoIndex)
,   m_propertylists(new QList<PropertyList *>())
,   m_empty_propertylist(new PropertyList())
{
    for (QString choice : choices)
        m_propertylists->append(m_empty_propertylist);
}

ListProperty::~ListProperty()
{
    delete m_propertylists;
    delete m_empty_propertylist;
}

void ListProperty::accept(AbstractPropertyVisitor & visitor)
{
    visitor.visitList(*this);
}

QString ListProperty::selectedChoice() const
{
    return m_selection != kNoIndex ? m_choices[m_selection] : "";
}

int ListProperty::selection() const
{
    return m_selection;
}

bool ListProperty::select(QString choice)
{
    int index = m_choices.indexOf(choice);
    if (index != kNoIndex) {
        m_selection = index;
        emit this->selectionChanged(*this);
        return true;
    } else
        return false;
}

bool ListProperty::select(int index)
{
    if (0 <= index && index < m_choices.size()) {
        m_selection = index;
        emit this->selectionChanged(*this);
        return true;
    } else {
        return false;
    }
}

QStringList ListProperty::choices() const
{
    return m_choices;
}

QList<PropertyList *> ListProperty::propertyLists() const
{
    return *m_propertylists;
}

bool ListProperty::add(QString choice)
{
    return this->add(choice, m_empty_propertylist);
}

bool ListProperty::add(QString choice, PropertyList * propertyList)
{
    int index = m_choices.indexOf(choice);
    if (index != kNoIndex) {
        return false;
    } else {
        m_choices.append(choice);
        m_propertylists->append(propertyList);
        emit this->choicesChanged(*this);
        return true;
    }
}

bool ListProperty::addList(QStringList choices)
{
    QStringList new_choices = m_choices + choices;
    if (new_choices.removeDuplicates() == 0) {
        m_choices = new_choices;
        emit this->choicesChanged(*this);
        return true;
    } else {
        return false;
    }
}

bool ListProperty::remove(QString choice)
{
    int index = m_choices.indexOf(choice);
    if (index != kNoIndex) {
        m_choices.removeAt(index);
        m_selection = kNoIndex;
        emit this->choicesChanged(*this);
        return true;
    } else
        return false;
}

bool ListProperty::remove(int index)
{
    if (0 <= index && index < m_choices.size()) {
        m_choices.removeAt(index);
        m_selection = kNoIndex;
        emit this->choicesChanged(*this);
        return true;
    } else {
        return false;
    }
}
