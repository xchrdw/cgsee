
#include <gui/propertywidgetbuilder.h>

#include <QFormLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QString>
#include <QGroupBox>
#include <QSlider>
#include <QSpinBox>
#include <QColorDialog>
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QStackedWidget>

#include <core/property/valueproperty.h>
#include <core/property/limitedproperty.h>
#include <core/property/advancedlistproperty.h>
#include <core/property/propertylist.h>


PropertyWidgetBuilder::PropertyWidgetBuilder()
: m_widget(new QWidget())
, m_layout(new QFormLayout(m_widget))
, m_active_widget(nullptr)
, m_active_layout(nullptr)
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
    m_widget->setWindowTitle("Painter Properties");
    m_layout = new QFormLayout(m_widget);
    m_layout->setSizeConstraint(QLayout::SetFixedSize);
    // m_layout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    m_widget->setLayout(m_layout);
    
    m_active_widget = m_widget;
    m_active_layout = m_layout;

    this->iterateOverProperties(properties);
}

void PropertyWidgetBuilder::visitList(ListProperty & property)
{
    QComboBox * comboBox = new QComboBox(m_active_widget);
    comboBox->addItems(property.choices());

    m_active_layout->addRow(property.description(), comboBox);

    QObject::connect(comboBox, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
        [&property] (const QString & text) {
            property.select(text);
        
            qDebug("Set Property %s = \"%s\"",
                   qPrintable(property.name()),
                   qPrintable(property.selectedChoice()));
        }
    );
}

void PropertyWidgetBuilder::visitAdvancedList(AdvancedListProperty & property)
{
    QComboBox * comboBox = new QComboBox(m_active_widget);
    comboBox->addItems(property.choices());

    m_active_layout->addRow(property.description(), comboBox);
    
    QStackedWidget * groupBoxesStack = new QStackedWidget(m_active_widget);
    m_active_layout->addRow(groupBoxesStack);

    for (PropertyList * propertyList : property.propertyLists()) {
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
        [&property, groupBoxesStack] (const QString & text) {
            property.select(text);
            groupBoxesStack->setCurrentIndex(property.selection());
        
            qDebug("Set Property %s = \"%s\"",
                   qPrintable(property.name()),
                   qPrintable(property.selectedChoice()));
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

void PropertyWidgetBuilder::visitValue(ValueProperty<QColor> & property)
{
    QHBoxLayout * layout = new QHBoxLayout(m_active_widget);

    QLabel * label = new QLabel("R: 0.1 G: 0.9 B: 0.22", m_active_widget);
    QPushButton * button = new QPushButton("Pick Color", m_active_widget);

    layout->addWidget(label);
    layout->addWidget(button);

    m_active_layout->addRow(property.description(), layout);

    QObject::connect(button, &QPushButton::clicked,
        [&property, label] (bool) {
            property.setValue(QColorDialog::getColor(property.value()));
            QString newlabel = QString("R: %1 G: %2 B: %3")
                                    .arg(property.value().red())
                                    .arg(property.value().green())
                                    .arg(property.value().blue());
            label->setText(newlabel);
            
            qDebug("Set Property %s = %s",
                   qPrintable(property.name()),
                   qPrintable(newlabel));
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
            qDebug("Set Property %s = %i",
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
            qDebug("Set Property %s = %.4f",
                   qPrintable(property.name()),
                   new_value);
        }
    );
}

QWidget * PropertyWidgetBuilder::retainWidget()
{
    QWidget * widget = m_widget;
    m_widget = nullptr;
    return widget;
}
