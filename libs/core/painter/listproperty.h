
#pragma once

#include <QStringList>
#include <core/declspec.h>
#include "abstractlistproperty.h"

class CGSEE_API ListProperty : public AbstractListProperty
{
public:
    ListProperty();
    explicit ListProperty(QString name);
    ListProperty(QString name, QString description);
    virtual ~ListProperty();

    virtual QStringList descriptionList() const;
    virtual QString selectedDescription() const;
    virtual bool select(QString description);

    bool remove(QString string);
    bool insert(QString string);
    bool insertList(QStringList strings);

protected:
    static const int kNoIndex = -1;
    QStringList * m_list;
    int m_selection;
};