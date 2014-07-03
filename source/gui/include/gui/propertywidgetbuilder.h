#pragma once

#include <gui/gui_api.h>

#include <core/property/abstractpropertyvisitor.h>

class QWidget;
class QFormLayout;

template <typename Type>
class ValueProperty;

class GUI_API PropertyWidgetBuilder : public AbstractPropertyVisitor
{
public:
    PropertyWidgetBuilder();
    virtual ~PropertyWidgetBuilder();

    void buildWidget(const QList<AbstractProperty *> & properties);

    virtual void visitList(ListProperty & property);
    virtual void visitAdvancedList(AdvancedListProperty & property);
    virtual void visitValue(ValueProperty<bool> & property);
    virtual void visitValue(ValueProperty<float> & property);
    virtual void visitValue(ValueProperty<int> & property);
    virtual void visitValue(ValueProperty<QString> & property);
    virtual void visitValue(ValueProperty<QColor> & property);
    virtual void visitLimited(LimitedProperty<int> & property);
    virtual void visitLimited(LimitedProperty<float> & property);

    QWidget * retainWidget();

protected:
    QWidget * m_widget;
    QFormLayout * m_layout;

    QWidget * m_active_widget;
    QFormLayout * m_active_layout;
};
