
#pragma once

#include <core/declspec.h>
#include <core/painter/abstractpropertyvisitor.h>

class QWidget;

class CGSEE_API PropertyWidgetBuilder : public AbstractPropertyVisitor
{
public:
    PropertyWidgetBuilder();
    virtual ~PropertyWidgetBuilder();

    virtual void visitBool(const BoolProperty & boolProperty);

    QWidget * retainWidget();

protected:
    QWidget * m_widget;
};