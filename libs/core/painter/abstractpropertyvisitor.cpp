
#include "abstractpropertyvisitor.h"
#include "valueproperty.h"

void AbstractPropertyVisitor::iterateOverProperties(const QList<AbstractPainterProperty *> & properties)
{
    for (int i = 0; i < properties.length(); i++)
        properties[i]->visit(*this);
}