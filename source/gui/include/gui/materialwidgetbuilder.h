#pragma once

#include <gui/gui_api.h>

class QWidget;
class Canvas;
class QDockWidget;

class GUI_API MaterialWidgetBuilder
{
public:
	MaterialWidgetBuilder();
	virtual ~MaterialWidgetBuilder();

public:
	virtual QWidget* initializeMaterialWidget(const GLFormat & format, QWidget * parent);

private:
	propertyguizeug::PropertyBrowser * m_propertyMaterialBrowser;
	Canvas * m_materialCanvas;
	QDockWidget * m_dockMaterial;
};