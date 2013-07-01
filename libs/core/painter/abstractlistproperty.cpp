
#include "abstractlistproperty.h"
#include "abstractpropertyvisitor.h"


AbstractListProperty::AbstractListProperty(QString name, QString description)
:   AbstractProperty(name, description)
{
}

AbstractListProperty::~AbstractListProperty()
{
}

void AbstractListProperty::visit(AbstractPropertyVisitor & visitor)
{
    visitor.visitList(*this);
}
