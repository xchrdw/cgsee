
#pragma once

#include <QString>
#include "abstractpainterproperty.h"

class AbstractListProperty : public AbstractPainterProperty
{
public:
    virtual QStringList keyList() const =0;
    virtual QString * selectedKey() const =0;
    virtual bool select(QString key) =0;

    virtual void visit(AbstractPropertyVisitor & visitor);
};