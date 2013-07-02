
#pragma once

#include <core/declspec.h>
#include <core/painter/abstractpropertyvisitor.h>

class QWidget;
class QFormLayout;

template <typename Type>
class ValueProperty;

class CGSEE_API PropertyWidgetBuilder : public AbstractPropertyVisitor
{
public:
    PropertyWidgetBuilder();
    virtual ~PropertyWidgetBuilder();

    void buildWidget(const QList<AbstractProperty *> & properties);

    virtual void visitList(AbstractListProperty & listProperty);
    virtual void visitValue(ValueProperty<bool> & property);
    virtual void visitValue(ValueProperty<float> & property);
    virtual void visitValue(ValueProperty<int> & property);
    virtual void visitLimited(LimitedProperty<int> & property);

    QWidget * retainWidget();

protected:
    QWidget * m_widget;
    QFormLayout * m_layout;
};
