
#pragma once

#include <QList>
#include <core/declspec.h>

class AbstractPainterProperty;
class BoolProperty;
class ListProperty;
class AbstractListProperty;

class CGSEE_API AbstractPropertyVisitor
{
public:
    virtual void visitBool(BoolProperty & boolProperty) =0;
    virtual void visitList(ListProperty & listProperty) =0;
    virtual void visitGenericList(AbstractListProperty & listProperty) =0;

protected:
    void iterateOverProperties(const QList<AbstractPainterProperty *> & properties);
};