
#include <core/property/abstractpropertyvisitor.h>

#include <core/property/valueproperty.h>


void AbstractPropertyVisitor::iterateOverProperties(const QList<AbstractProperty *> & properties)
{
    for (int i = 0; i < properties.length(); i++)
        properties[i]->accept(*this);
}
