
#include "abstractlistproperty.h"
#include "abstractpropertyvisitor.h"

AbstractListProperty::AbstractListProperty()
:   AbstractPainterProperty()
{
}

AbstractListProperty::AbstractListProperty(QString name)
:   AbstractPainterProperty(name)
{
}

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
