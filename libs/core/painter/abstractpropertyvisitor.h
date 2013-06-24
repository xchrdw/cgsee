
#pragma once

#include <QList>
#include <core/declspec.h>

class AbstractPainterProperty;

class CGSEE_API AbstractPropertyVisitor
{
public:
    void iterateOverProperties(const QList<AbstractPainterProperty *> & properties);
};