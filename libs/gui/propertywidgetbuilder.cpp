
#include <QFormLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QString>
#include <QGroupBox>
#include <QSlider>
#include <QSpinBox>
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QStackedWidget>
#include "propertywidgetbuilder.h"
#include <core/property/valueproperty.h>
#include <core/property/limitedproperty.h>
#include <core/property/listproperty.h>
#include <core/property/propertylist.h>

PropertyWidgetBuilder::PropertyWidgetBuilder()
:   m_widget(new QWidget())
,   m_layout(new QFormLayout(m_widget))
,   m_active_widget(nullptr)
,   m_active_layout(nullptr)
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
    m_layout->setSizeConstraint(QLayout::SetFixedSize);
    m_layout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    m_widget->setLayout(m_layout);
    
    m_active_widget = m_widget;
    m_active_layout = m_layout;

    this->iterateOverProperties(properties);
}

void PropertyWidgetBuilder::visitList(ListProperty & listProperty)
{
    QComboBox * comboBox = new QComboBox(m_active_widget);
    comboBox->addItems(listProperty.choices());

    m_active_layout->addRow(listProperty.description(), comboBox);
    
    QStackedWidget * groupBoxesStack = new QStackedWidget(m_active_widget);
    m_active_layout->addRow(groupBoxesStack);

    for (PropertyList * propertyList : listProperty.propertyLists()) {
        QGroupBox * groupBox = new QGroupBox(m_active_widget);
        groupBox->setAlignment(Qt::AlignVCenter);
        QFormLayout * groupboxlayout = new QFormLayout(groupBox);
        
        if (propertyList->isEmpty()) {
            QLabel * label = new QLabel("No Properties to display", groupBox);
            label->setAlignment(Qt::AlignCenter);
            groupboxlayout->addWidget(label);
        }
        
        
        QWidget * former_active_widget = m_active_widget;
        QFormLayout * former_active_layout = m_active_layout;
        
        m_active_widget = groupBox;
        m_active_layout = groupboxlayout;
        
        this->iterateOverProperties(propertyList->list());
        
        m_active_widget = former_active_widget;
        m_active_layout = former_active_layout;
        
        groupBoxesStack->addWidget(groupBox);
    }

    QObject::connect(comboBox, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
        [&listProperty, groupBoxesStack] (const QString & text) {
            listProperty.select(text);
            groupBoxesStack->setCurrentIndex(listProperty.selection());
        
            qDebug("Painter: Set %s = \"%s\"",
                   qPrintable(listProperty.name()),
                   qPrintable(listProperty.selectedChoice()));
        }
    );
}

void PropertyWidgetBuilder::visitValue(ValueProperty<float> & property)
{
    QSlider * slider = new QSlider(m_active_widget);
    slider->setOrientation(Qt::Horizontal);

    m_active_layout->addRow(property.description(), slider);

    QObject::connect(slider, &QSlider::valueChanged,
        [&property] (int i) {
            property.setValue(i);
            qDebug("Set Property %s = %i", 
                   qPrintable(property.name()), 
                   i);
        }
    );
}

void PropertyWidgetBuilder::visitValue(ValueProperty<bool> & property)
{
    QCheckBox * checkBox = new QCheckBox(m_active_widget);
    checkBox->setText(property.description());
    if (property.value())
        checkBox->setCheckState(Qt::Checked);
    else
        checkBox->setCheckState(Qt::Unchecked);

    m_active_layout->addWidget(checkBox);

    QObject::connect(checkBox, &QCheckBox::stateChanged, 
        [&property] (int state) {
            property.setValue(state);
            qDebug("Set Property %s = %i", 
                   qPrintable(property.name()), 
                   property.value());
        }
    );
}

void PropertyWidgetBuilder::visitValue(ValueProperty<int> & property)
{
    QSpinBox * spinbox = new QSpinBox(m_active_widget);
    m_active_layout->addRow(property.description(), spinbox);

    QObject::connect(spinbox,
        static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
        [&property] (int i) {
            property.setValue(i);
            qDebug("Set Property %s = %i", qPrintable(property.name()), i);
        }
    );
}

void PropertyWidgetBuilder::visitValue(ValueProperty<QString> & property)
{
    QLineEdit * lineEdit = new QLineEdit(property.value(), m_active_widget);

    m_active_layout->addRow(property.description(), lineEdit);

    QObject::connect(lineEdit, &QLineEdit::textChanged,
        [&property] (QString text) {
            property.setValue(text);
            qDebug("Set Property %s = \"%s\"", 
                   qPrintable(property.name()), 
                   qPrintable(text));
        }
    );
}

void PropertyWidgetBuilder::visitLimited(LimitedProperty<int> & property)
{
    QHBoxLayout * layout = new QHBoxLayout(m_active_widget);

    QLabel * minLabel = new QLabel(QString::number(property.minimum()), m_active_widget);
    QLabel * maxLabel = new QLabel(QString::number(property.maximum()), m_active_widget);

    QSlider * slider = new QSlider(m_active_widget);
    slider->setOrientation(Qt::Horizontal);

    slider->setValue(property.value());
    slider->setMinimum(property.minimum());
    slider->setMaximum(property.maximum());
    
    layout->addWidget(minLabel);
    layout->addWidget(slider);
    layout->addWidget(maxLabel);
    m_active_layout->addRow(property.description(), layout);

    QObject::connect(slider, &QSlider::valueChanged,
        [&property] (int i) {
            property.setValue(i);
            qDebug("Painter: Set %s = %i", 
                   qPrintable(property.name()), 
                   i);
        }
    );
}

void PropertyWidgetBuilder::visitLimited(LimitedProperty<float> & property)
{
    QLabel * minLabel = new QLabel(QString::number(property.minimum(), 'g', 2));
    QLabel * maxLabel = new QLabel(QString::number(property.maximum(), 'g', 2));

    QSlider * slider = new QSlider(m_active_widget);
    slider->setOrientation(Qt::Horizontal);
    
    float range = property.maximum() - property.minimum();
    float scale_factor = 100 / range;

    slider->setValue((property.value() - property.minimum()) * scale_factor);
    slider->setMinimum(0.f);
    slider->setMaximum(100.f);
    
    QHBoxLayout * layout = new QHBoxLayout(m_active_widget);
    layout->addWidget(minLabel);
    layout->addWidget(slider);
    layout->addWidget(maxLabel);
    m_active_layout->addRow(property.description(), layout);

    QObject::connect(slider, &QSlider::valueChanged,
        [&property, scale_factor] (int i) {
            float new_value = (i / scale_factor) + property.minimum();
            property.setValue(new_value);
            qDebug("Painter: Set %s = %.4f", 
                   qPrintable(property.name()), 
                   new_value);
        }
    );
}

QWidget * PropertyWidgetBuilder::retainWidget()
{
    QWidget * widget = m_widget;
    widget->setWindowTitle("Painter Properties");
    m_widget = nullptr;
    return widget;
}