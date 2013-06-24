
#pragma once

#include <QList>
#include <core/declspec.h>

class AbstractPainterProperty;
class BoolProperty;

class CGSEE_API AbstractPropertyVisitor
{
public:
    virtual void visitBool(const BoolProperty & boolProperty) =0;

protected:
    void iterateOverProperties(const QList<AbstractPainterProperty *> & properties);
};