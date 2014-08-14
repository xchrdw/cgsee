#include <gui/lightwidgetbuilder.h>

#include <qwidget.h>
#include <qboxlayout.h>
#include <qdockwidget.h>
#include <qmainwindow.h>
#include <propertyguizeug/PropertyBrowser.h>

#include <core/rendering/lightrepresentation/pointlight.h>

class AbstractLight;

LightWidgetBuilder::LightWidgetBuilder(QWidget * parent)
: m_lightWidget(nullptr)
, m_dockLight(new QDockWidget(QMainWindow::tr("Material")))
, m_propertyLightBrowser(nullptr)
{
	m_lightWidget = new QWidget(parent);
}

LightWidgetBuilder::~LightWidgetBuilder()
{
	delete m_lightWidget;
	delete m_dockLight;
	delete m_propertyLightBrowser;
}

void LightWidgetBuilder::initializeLightWidget()
{
	m_dockLight->setObjectName("lightWidget");

	//TODO
	AbstractLight * obj = new PointLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(5.0f, 5.0f, 5.0f), 4.0f);

	m_propertyLightBrowser = new propertyguizeug::PropertyBrowser(obj);

	QVBoxLayout * layout = new QVBoxLayout();
	layout->addWidget(m_propertyLightBrowser);

	m_lightWidget->setLayout(layout);
}

QDockWidget * LightWidgetBuilder::getDockLight()
{
	return m_dockLight;
}

QWidget * LightWidgetBuilder::getLightWidget()
{
	return m_lightWidget;
}

propertyguizeug::PropertyBrowser * LightWidgetBuilder::getPropertyBrowser()
{
	return m_propertyLightBrowser;
}