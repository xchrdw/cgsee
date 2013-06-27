
#pragma once

#include <core/declspec.h>
#include <core/painter/abstractpropertyvisitor.h>

class QWidget;
class QFormLayout;

class CGSEE_API PropertyWidgetBuilder : public AbstractPropertyVisitor
{
public:
    PropertyWidgetBuilder();
    virtual ~PropertyWidgetBuilder();

    void buildWidget(const QList<AbstractPainterProperty *> & properties);

    virtual void visitBool(BoolProperty & boolProperty);
    virtual void visitList(AbstractListProperty & listProperty);
    virtual void visitInt(IntProperty & intProperty);
    virtual void visitFloat(FloatProperty & floatProperty);

    QWidget * retainWidget();

protected:
    QWidget * m_widget;
    QFormLayout * m_layout;
};
