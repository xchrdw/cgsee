
#include "genericlistproperty.h"

GenericListProperty::GenericListProperty()
:   AbstractPainterProperty()
,   m_map(new QMap<QString, ValueClass)
,   m_selectedKey(nullptr)
{
}

GenericListProperty::GenericListProperty(QString name)
:   AbstractPainterProperty(name)
,   m_map(new QMap<QString, ValueClass)
,   m_selectedKey(nullptr)
{
}

GenericListProperty::GenericListProperty(QString name, QString, description)
:   AbstractPainterProperty(name, description)
,   m_map(new QMap<QString, ValueClass)
,   m_selectedKey(nullptr)
{
}

GenericListProperty::~GenericListProperty()
{
    delete m_map;
    delete m_selectedKey;
}

QStringList GenericListProperty::keyList() const
{
}

QString * GenericListProperty::selectedKey() const
{
}

bool GenericListProperty::select(QString key)
{
}

QList<ValueClass> GenericListProperty::valueList()
{
}

ValueClass * GenericListProperty::selectedValue()
{
}

bool GenericListProperty::select(ValueClass * value)
{
}

bool GenericListProperty::insert(QString key, ValueClass * value)
{
}

bool GenericListProperty::remove(QString key)
{
}

bool GenericListProperty::remove(ValueClass * value)
{
}
