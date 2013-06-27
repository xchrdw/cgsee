
#include <QFormLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QString>
#include <QSlider>
#include <QSpinBox>
#include "propertywidgetbuilder.h"
#include <core/painter/intproperty.h>
#include <core/painter/floatproperty.h>
#include <core/painter/boolproperty.h>
#include <core/painter/abstractlistproperty.h>

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
    QCheckBox * checkBox = new QCheckBox(m_widget);
    if (boolProperty.enabled())
        checkBox->setCheckState(Qt::Checked);
    else
        checkBox->setCheckState(Qt::Unchecked);

    m_layout->addRow(boolProperty.description(), checkBox);

    QObject::connect(checkBox, &QCheckBox::stateChanged, [&boolProperty] (int state) {
        boolProperty.setEnabled(state);
        qDebug("Painter: Set %s = %i", qPrintable(boolProperty.name()), boolProperty.enabled());
    });
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

void PropertyWidgetBuilder::visitInt(IntProperty & intProperty)
{
    QSpinBox * spinbox = new QSpinBox(m_widget);
    m_layout->addRow(intProperty.description(), spinbox);

    QObject::connect(spinbox,
                     static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
                     [&intProperty] (int i) {
                         intProperty.setValue(i);
                         qDebug("Painter: Set %s = %i",
                                qPrintable(intProperty.name()),
                                i);
                     });
}

void PropertyWidgetBuilder::visitFloat(FloatProperty & floatProperty)
{
    QSlider * slider = new QSlider(m_widget);
    slider->setOrientation(Qt::Horizontal);

    m_layout->addRow(floatProperty.description(), slider);

    QObject::connect(slider,
                     static_cast<void (QSlider::*)(int)>(&QSlider::valueChanged),
                     [&floatProperty] (int i) {
                         floatProperty.setValue(i);
                         qDebug("Painter: Set %s = %i",
                                qPrintable(floatProperty.name()),
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
