
#include "listproperty.h"
#include "abstractpropertyvisitor.h"

ListProperty::ListProperty(QString name, QString description)
:   AbstractProperty(name, description)
,   m_selection(kNoIndex)
{
}

ListProperty::ListProperty(QString name, QString description, QStringList choices)
:   AbstractProperty(name, description)
,   m_choices(choices)
{
}

ListProperty::~ListProperty()
{
}

QString ListProperty::selectedChoice() const
{
    return m_selection != kNoIndex ? m_choices[m_selection] : "";
}

int ListProperty::selection() const
{
    return m_selection;
}

void ListProperty::accept(AbstractPropertyVisitor & visitor)
{
    visitor.visitList(*this);
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

bool ListProperty::add(QString choice)
{
    int index = m_choices.indexOf(choice);
    if (index != kNoIndex) {
        return false;
    } else {
        m_choices.append(choice);
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
        return remove(index);
    } else {
        return false;
    }
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