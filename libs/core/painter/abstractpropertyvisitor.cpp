
#include "abstractpropertyvisitor.h"
#include "abstractpainterproperty.h"

AbstractPainterProperty::AbstractPainterProperty()
:   m_name("")
{
}
    
AbstractPainterProperty::AbstractPainterProperty(QString name)
:   m_name(name)
{
}

void AbstractPropertyVisitor::iterateOverProperties(const QList<AbstractPainterProperty *> & properties)
{
    for (int i = 0; i < properties.length(); i++)
        properties[i]->visit(*this);
}