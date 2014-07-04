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

class GUI_API MaterialWidgetBuilder
{
public:
	MaterialWidgetBuilder(QWidget * parent);
	virtual ~MaterialWidgetBuilder();

public:
	virtual void initializeMaterialWidget(const GLFormat & format);
	QDockWidget * getDockMaterial();
	QWidget * getMaterialWidget();
	propertyguizeug::PropertyBrowser * getPropertyBrowser();

private:
	propertyguizeug::PropertyBrowser * m_propertyMaterialBrowser;
	Canvas * m_materialCanvas;
	QDockWidget * m_dockMaterial;
	QWidget * m_materialWidget;
};