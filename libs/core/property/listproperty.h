
#pragma once

#include <QStringList>
#include <core/declspec.h>
#include "abstractproperty.h"

class PropertyList;

class CGSEE_API ListProperty : public AbstractProperty
{
    Q_OBJECT
public:
    ListProperty(QString name, QString description);
    ListProperty(QString name, QString description, QStringList choices);
    virtual ~ListProperty();

    virtual void accept(AbstractPropertyVisitor & visitor);

    QString selectedChoice() const;
    int selection() const;

    bool select(QString choice);
    bool select(int index);

    QStringList choices() const;
    QList<PropertyList *> propertyLists() const;  

    bool add(QString choice);
    bool add(QString choice, PropertyList * propertyList);
    bool addList(QStringList choices);
    bool remove(QString choice);
    bool remove(int index);

signals:
    void selectionChanged(ListProperty & me);
    void choicesChanged(ListProperty & me);

protected:
    static const int kNoIndex = -1;
    QStringList m_choices;
    int m_selection;
    QList<PropertyList *> * m_propertylists;
    PropertyList * m_empty_propertylist;
};
