
#include "abstractlistproperty.h"
#include "abstractpropertyvisitor.h"


AbstractListProperty::AbstractListProperty(QString name, QString description)
:   AbstractPainterProperty(name, description)
{
}

AbstractListProperty::~AbstractListProperty()
{
}

void AbstractListProperty::visit(AbstractPropertyVisitor & visitor)
{
    visitor.visitList(*this);
}
