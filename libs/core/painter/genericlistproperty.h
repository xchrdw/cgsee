
#pragma once

#include <QMap>
#include <QString>
#include "abstractlistproperty.h"

template <class ValueClass>
class GenericListProperty : public AbstractListProperty
{
public:
    GenericListProperty();
    GenericListProperty(QString name);
    GenericListProperty(QString name, QString, description);
    ~GenericListProperty();

    virtual QStringList keyList() const;
    virtual QString * selectedKey() const;
    virtual bool select(QString key);

    QList<ValueClass> valueList();
    ValueClass * selectedValue();

    bool select(ValueClass * value);
    bool insert(QString key, ValueClass * value);
    bool remove(QString key);
    bool remove(ValueClass * value);

protected:
    QMap<QString, ValueClass *> * m_map;
    QString * m_selectedKey;
}