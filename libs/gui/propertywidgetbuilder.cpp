
#include <QFormLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QString>
#include <QSlider>
#include <QSpinBox>
#include <QLabel>
#include <QHBoxLayout>
#include "propertywidgetbuilder.h"
#include <core/property/valueproperty.h>
#include <core/property/limitedproperty.h>
#include <core/property/listproperty.h>

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

void PropertyWidgetBuilder::visitList(ListProperty & listProperty)
{
    QComboBox * comboBox = new QComboBox(m_widget);
    comboBox->addItems(listProperty.choices());

    m_layout->addRow(listProperty.description(), comboBox);

    QObject::connect(comboBox,
                     static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
                     [&listProperty] (const QString & text) {
                         listProperty.select(text);
                         qDebug("Painter: Set %s = %s",
                                qPrintable(listProperty.name()),
                                qPrintable(listProperty.selectedChoice()));
                     });
}

void PropertyWidgetBuilder::visitValue(ValueProperty<float> & property)
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

void PropertyWidgetBuilder::visitValue(ValueProperty<bool> & property)
{
    QCheckBox * checkBox = new QCheckBox(m_widget);
    checkBox->setText(property.description());
    if (property.value())
        checkBox->setCheckState(Qt::Checked);
    else
        checkBox->setCheckState(Qt::Unchecked);

    m_layout->addRow("", checkBox);

    QObject::connect(checkBox, &QCheckBox::stateChanged, [&property] (int state) {
        property.setValue(state);
        qDebug("Painter: Set %s = %i", qPrintable(property.name()), property.value());
    });
}

void PropertyWidgetBuilder::visitValue(ValueProperty<int> & property)
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

void PropertyWidgetBuilder::visitLimited(LimitedProperty<float> & property)
{
    
    QLabel * minLabel = new QLabel(QString::number(property.minimum(), 'g', 2));
    QLabel * maxLabel = new QLabel(QString::number(property.maximum(), 'g', 2));

    QSlider * slider = new QSlider(m_widget);
    slider->setOrientation(Qt::Horizontal);
    
    float range = property.maximum() - property.minimum();
    float scale_factor = 100 / range;

    slider->setValue((property.value() - property.minimum()) * scale_factor);
    slider->setMinimum(0.f);
    slider->setMaximum(100.f);
    
    QHBoxLayout * layout = new QHBoxLayout(m_widget);
    layout->addWidget(minLabel);
    layout->addWidget(slider);
    layout->addWidget(maxLabel);
    m_layout->addRow(property.description(), layout);

    QObject::connect(slider, &QSlider::valueChanged,
        [&property, scale_factor] (int i) {
             float new_value = (i / scale_factor) + property.minimum();
             property.setValue(new_value);
             qDebug("Painter: Set %s = %.4f",
                    qPrintable(property.name()),
                    new_value);
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
