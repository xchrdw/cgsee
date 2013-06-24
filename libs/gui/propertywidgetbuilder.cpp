
#include "propertywidgetbuilder.h"
#include <QWidget>

PropertyWidgetBuilder::PropertyWidgetBuilder()
:   m_widget(new QWidget())
{
}

PropertyWidgetBuilder::~PropertyWidgetBuilder()
{
}

void PropertyWidgetBuilder::visitBool(const BoolProperty & boolProperty)
{
    // do stuff;
}