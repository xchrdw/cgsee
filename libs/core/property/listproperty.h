
#pragma once

#include <QStringList>
#include <core/declspec.h>
#include "abstractproperty.h"

class CGSEE_API ListProperty : public AbstractProperty
{
public:
    ListProperty(QString name, QString description);
    ListProperty(QString name, QString description, QStringList choices);
    virtual ~ListProperty();

    virtual void visit(AbstractPropertyVisitor & visitor);

    QString selectedChoice() const;
    int selection() const;

    bool select(QString choice);
    bool select(int index);

    QStringList choices() const;
    
    bool add(QString choice);
    bool addList(QStringList choices);
    bool remove(QString choice);
    bool remove(int index);

protected:
    static const int kNoIndex = -1;
    QStringList m_choices;
    int m_selection;
};
