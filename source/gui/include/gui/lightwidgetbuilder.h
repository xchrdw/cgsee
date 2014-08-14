#pragma once

#include <gui/gui_api.h>

class QWidget;
class Canvas;
class QDockWidget;
class GLFormat;

namespace propertyguizeug
{
	class PropertyBrowser;
}

class GUI_API LightWidgetBuilder
{
public:
	LightWidgetBuilder(QWidget * parent);
	virtual ~LightWidgetBuilder();

public:
	virtual void initializeLightWidget();
	QDockWidget * getDockLight();
	QWidget * getLightWidget();
	propertyguizeug::PropertyBrowser * getPropertyBrowser();

private:
	propertyguizeug::PropertyBrowser * m_propertyLightBrowser;
	Canvas * m_lightCanvas;
	QDockWidget * m_dockLight;
	QWidget * m_lightWidget;
};