
#include <QFormLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QString>
#include <QSlider>
#include <QSpinBox>
#include "propertywidgetbuilder.h"
#include <core/painter/abstractlistproperty.h>
#include <core/painter/valueproperty.h>
#include <core/painter/limitedproperty.h>

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

void PropertyWidgetBuilder::buildWidget(const QList<AbstractProperty *> & properties)
{
    delete m_widget;
    m_widget = new QWidget();
    m_layout = new QFormLayout(m_widget);
    m_widget->setLayout(m_layout);

    this->iterateOverProperties(properties);
}

void PropertyWidgetBuilder::visitList(AbstractListProperty & listProperty)
{
    QComboBox * comboBox = new QComboBox(m_widget);
    comboBox->addItems(listProperty.descriptionList());

    m_layout->addRow(listProperty.description(), comboBox);

    QObject::connect(comboBox,
                     static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
                     [&listProperty] (const QString & text) {
                         listProperty.select(text);
                         qDebug("Painter: Set %s = %s",
                                qPrintable(listProperty.name()),
                                qPrintable(listProperty.selectedDescription()));
                     });
}

void PropertyWidgetBuilder::visitGeneric(ValueProperty<float> & property)
{
    QSlider * slider = new QSlider(m_widget);
    slider->setOrientation(Qt::Horizontal);

    m_layout->addRow(property.description(), slider);

    QObject::connect(slider,
                     static_cast<void (QSlider::*)(int)>(&QSlider::valueChanged),
                     [&property] (int i) {
                         property.setValue(i);
                         qDebug("Painter: Set %s = %i",
                                qPrintable(property.name()),
                                i);
                     });
}

void PropertyWidgetBuilder::visitGeneric(ValueProperty<bool> & property)
{
    QCheckBox * checkBox = new QCheckBox(m_widget);
    if (property.value())
        checkBox->setCheckState(Qt::Checked);
    else
        checkBox->setCheckState(Qt::Unchecked);

    m_layout->addRow(property.description(), checkBox);

    QObject::connect(checkBox, &QCheckBox::stateChanged, [&property] (int state) {
        property.setValue(state);
        qDebug("Painter: Set %s = %i", qPrintable(property.name()), property.value());
    });
}

void PropertyWidgetBuilder::visitGeneric(ValueProperty<int> & property)
{
    QSpinBox * spinbox = new QSpinBox(m_widget);
    m_layout->addRow(property.description(), spinbox);

    QObject::connect(spinbox,
                     static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
                     [&property] (int i) {
                         property.setValue(i);
                         qDebug("Painter: Set %s = %i",
                                qPrintable(property.name()),
                                i);
                     });
}

void PropertyWidgetBuilder::visitLimited(LimitedProperty<int> & property)
{
    QSpinBox * spinbox = new QSpinBox(m_widget);
    spinbox->setMinimum(property.minimum());
    spinbox->setMaximum(property.maximum());
    m_layout->addRow(property.description(), spinbox);

    QObject::connect(spinbox,
                     static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
                     [&property] (int i) {
                         property.setValue(i);
                         qDebug("Painter: Set %s = %i",
                                qPrintable(property.name()),
                                i);
                     });
}

QWidget * PropertyWidgetBuilder::retainWidget()
{
    QWidget * widget = m_widget;
    widget->setWindowTitle("Painter Properties");
    widget->setWindowFlags(Qt::Tool);
    m_widget = nullptr;
    return widget;
}
