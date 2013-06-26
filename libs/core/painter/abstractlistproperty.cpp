
#include "abstractlistproperty.h"
#include "abstractpropertyvisitor.h"

void AbstractListProperty::visit(AbstractPropertyVisitor & visitor)
{
    visitor.visitGenericList(*this);
}
