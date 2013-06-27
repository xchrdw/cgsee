
#pragma once

#include <QMap>
#include <QString>
#include "abstractlistproperty.h"

template <class ValueClass>
class GenericListProperty : public AbstractListProperty
{
public:
    GenericListProperty(QString name, QString description);
    ~GenericListProperty();

    virtual QStringList descriptionList() const;
    virtual QString selectedDescription() const;
    virtual bool select(QString description);

    QList<ValueClass> valueList();
    ValueClass * selectedValue();

    bool select(ValueClass * value);
    bool insert(QString key, ValueClass * value);
    bool remove(QString key);
    bool remove(ValueClass * value);

protected:
    ValueClass * value(QString key);

protected:
    QMap<QString, ValueClass *> * m_map;
    QString * m_selectedKey;
};

#include "genericlistproperty-inl.h"
