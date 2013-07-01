
#include "abstractpropertyvisitor.h"
#include "genericproperty.h"

void AbstractPropertyVisitor::visitGeneric(GenericProperty<int> & intProperty)
{
    qDebug("visit int property");
}

void AbstractPropertyVisitor::visitGeneric(GenericProperty<float> & intProperty)
{
    qDebug("visit float property");
}

void AbstractPropertyVisitor::iterateOverProperties(const QList<AbstractPainterProperty *> & properties)
{
    for (int i = 0; i < properties.length(); i++)
        properties[i]->visit(*this);
}