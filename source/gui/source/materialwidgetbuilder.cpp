#include <gui/materialwidgetbuilder.h>

#include <qwidget.h>
#include <qboxlayout.h>
#include <qdockwidget.h>
#include <qmainwindow.h>
#include <propertyguizeug/PropertyBrowser.h>

#include <core/material/material.h>
#include <core/glformat.h>
#include <gui/canvas.h>

MaterialWidgetBuilder::MaterialWidgetBuilder(QWidget * parent)
:   m_materialWidget(nullptr)
,   m_dockMaterial(new QDockWidget(QMainWindow::tr("Material")))
,   m_materialCanvas(nullptr)
,   m_propertyMaterialBrowser(nullptr)
{
	m_materialWidget = new QWidget(parent);
}

MaterialWidgetBuilder::~MaterialWidgetBuilder()
{
	delete m_materialWidget;
	delete m_dockMaterial;
	delete m_materialCanvas;
	delete m_propertyMaterialBrowser;
}

void MaterialWidgetBuilder::initializeMaterialWidget(const GLFormat & format)
{
	m_dockMaterial->setObjectName("materialWidget");
	
	Material * obj = new Material();

	m_propertyMaterialBrowser = new propertyguizeug::PropertyBrowser(obj);

	m_materialCanvas = new Canvas(format);
	// ToDo: add Vertical splitter layout and handle this 
	m_materialCanvas->setMinimumHeight(128);	

	QVBoxLayout * layout = new QVBoxLayout();
	layout->addWidget(m_materialCanvas);
	layout->addWidget(m_propertyMaterialBrowser);

	m_materialWidget->setLayout(layout);
}

QDockWidget * MaterialWidgetBuilder::getDockMaterial()
{
	return m_dockMaterial;
}

QWidget * MaterialWidgetBuilder::getMaterialWidget()
{
	return m_materialWidget;
}

propertyguizeug::PropertyBrowser * MaterialWidgetBuilder::getPropertyBrowser()
{
	return m_propertyMaterialBrowser;
}
