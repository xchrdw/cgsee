
#include "listproperty.h"

#include "abstractpropertyvisitor.h"

ListProperty::ListProperty()
:   AbstractPainterProperty()
,   m_list(new QStringList())
,   m_selection(kNoIndex)
{
}

ListProperty::ListProperty(QString name)
:   AbstractPainterProperty(name)
,   m_list(new QStringList())
,   m_selection(kNoIndex)
{
}

ListProperty::ListProperty(QString name, QString description)
:   AbstractPainterProperty(name, description)
,   m_list(new QStringList())
,   m_selection(kNoIndex)
{
}

ListProperty::~ListProperty()
{
    delete m_list;
}

void ListProperty::visit(AbstractPropertyVisitor & visitor)
{
    visitor.visitList(*this);
}

ListProperty * ListProperty::toList()
{
    return this;
}

QStringList & ListProperty::list() const
{
    return *m_list;
}
QString ListProperty::selection() const
{
    if (m_selection != kNoIndex)
        return m_list->at(m_selection);
    else
        return "";
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

bool ListProperty::select(QString string)
{
    int index = m_list->indexOf(string);
    if (index != kNoIndex) {
        m_selection = index;
        return true;
    } else
        return false;
}