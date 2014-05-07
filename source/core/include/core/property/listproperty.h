#pragma once

#include <core/core_api.h>

#include <QStringList>

#include <core/property/abstractproperty.h>

class CORE_API ListProperty : public AbstractProperty
{
public:
    typedef enum { kSelectionChanged, kChoicesChanged } Events;

    ListProperty(QString name, QString description);
    ListProperty(QString name, QString description, QStringList choices);
    virtual ~ListProperty();
    
    virtual void accept(AbstractPropertyVisitor & visitor);

    QString selectedChoice() const;
    int selection() const;

    bool select(QString choice);
    bool select(int index);

    QStringList choices() const;

    virtual bool add(QString choice);
    virtual bool addList(QStringList choices);
    virtual bool remove(QString choice);
    virtual bool remove(int index);

protected:
    static const int kNoIndex = -1;
    QStringList m_choices;
    int m_selection;
};
