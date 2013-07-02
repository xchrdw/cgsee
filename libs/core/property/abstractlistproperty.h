
#pragma once

#include <QString>
#include "abstractproperty.h"

class AbstractListProperty : public AbstractProperty
{
public:
    AbstractListProperty(QString name, QString description);
    virtual ~AbstractListProperty();

    virtual QStringList descriptionList() const = 0;
    virtual QString selectedDescription() const = 0;
    virtual bool select(QString description) = 0;

    virtual void visit(AbstractPropertyVisitor & visitor);
};
