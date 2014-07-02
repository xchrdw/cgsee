#include <gui/materialwidgetbuilder.h>

#include <qwidget.h>
#include <qboxlayout.h>
#include <propertyguizeug/PropertyBrowser.h>

#include <core/material/material.h>
#include <gui/canvas.h>

MaterialWidgetBuilder::MaterialWidgetBuilder()
{

}

MaterialWidgetBuilder::~MaterialWidgetBuilder()
{

}

QWidget* MaterialWidgetBuilder::initializeMaterialWidget(const GLFormat & format, QWidget * parent)
{
	Material * obj = new Material();

	m_propertyMaterialBrowser = new propertyguizeug::PropertyBrowser(obj);

	m_materialCanvas = new Canvas(format);
	// ToDo: add Vertical splitter layout and handle this 
	m_materialCanvas->setMinimumHeight(128);	

	QVBoxLayout * layout = new QVBoxLayout();
	layout->addWidget(m_materialCanvas);
	layout->addWidget(m_propertyMaterialBrowser);

	QWidget * materialWidget = new QWidget(parent);
	materialWidget->setLayout(layout);

	return materialWidget;
}