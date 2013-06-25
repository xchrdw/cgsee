
#include <QFormLayout>
#include <QCheckBox>
#include <QString>
#include "propertywidgetbuilder.h"
#include <core/painter/boolproperty.h>

PropertyWidgetBuilder::PropertyWidgetBuilder()
:   m_widget(new QWidget())
,   m_layout(new QFormLayout(m_widget))
{
    m_widget->setLayout(m_layout);
}

PropertyWidgetBuilder::~PropertyWidgetBuilder()
{
    delete m_widget;
}

void PropertyWidgetBuilder::buildWidget(const QList<AbstractPainterProperty *> & properties)
{
    delete m_widget;
    m_widget = new QWidget();
    m_layout = new QFormLayout(m_widget);
    m_widget->setLayout(m_layout);

    this->iterateOverProperties(properties);
}

void PropertyWidgetBuilder::visitBool(BoolProperty & boolProperty)
{
    QCheckBox * box = new QCheckBox(boolProperty.name(), m_widget);
    m_layout->addWidget(box);
    QObject::connect(box, &QCheckBox::stateChanged, [&boolProperty] (int state) -> void {
        boolProperty.setEnabled(state);
        qDebug("Painte: Set %s = %i", qPrintable(boolProperty.name()), boolProperty.enabled());
    });
}

QWidget * PropertyWidgetBuilder::retainWidget()
{
    QWidget * widget = m_widget;
    m_widget = nullptr;
    return widget;
}