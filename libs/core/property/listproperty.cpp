
#include "listproperty.h"

#include "abstractpropertyvisitor.h"

ListProperty::ListProperty(QString name, QString description)
:   AbstractListProperty(name, description)
,   m_list(new QStringList())
,   m_selection(kNoIndex)
{
}

ListProperty::~ListProperty()
{
    delete m_list;
}

QStringList ListProperty::descriptionList() const
{
    return *m_list;
}
QString ListProperty::selectedDescription() const
{
    return m_selection != kNoIndex ? m_list->at(m_selection) : "";
}

bool ListProperty::select(QString description)
{
    int index = m_list->indexOf(description);
    if (index != kNoIndex) {
        m_selection = index;
        return true;
    } else
        return false;
}

bool ListProperty::insert(QString string)
{
    int index = m_list->indexOf(string);
    if (index != kNoIndex) {
        return false;
    } else
        m_list->append(string);
        return true;
}

bool ListProperty::insertList(QStringList strings)
{
    bool success = true;

    for (QString string : strings)
        if (!(success = this->insert(string)))
            break;

    if (!success)
        for (QString string : strings)
            this->remove(string);

    return success;
}

bool ListProperty::remove(QString string)
{
    int index = m_list->indexOf(string);
    if (index != kNoIndex) {
        m_list->removeAt(index);
        m_selection = kNoIndex;
        return true;
    } else
        return false;
}
