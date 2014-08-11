#pragma once

#include <gui/gui_api.h>

class QWidget;
class Canvas;
class QDockWidget;
class GLFormat;
class AbstractModelLoader;
class Group;
class AbstractScenePainter;
class QString;
class Camera;

namespace propertyguizeug
{
	class PropertyBrowser;
}

class GUI_API MaterialWidgetBuilder
{
public:
	MaterialWidgetBuilder(QWidget * parent, AbstractModelLoader * loader);
	virtual ~MaterialWidgetBuilder();

public:
	virtual void initializeMaterialWidget(const GLFormat & format, Camera * ncamera);
	QDockWidget * getDockMaterial();
	QWidget * getMaterialWidget();
	propertyguizeug::PropertyBrowser * getPropertyBrowser();
	void setPainter(AbstractScenePainter * painter);
	AbstractScenePainter * painter();

protected:
	void loadFile(const QString & path);

protected:
	propertyguizeug::PropertyBrowser * m_propertyMaterialBrowser;
	Canvas * m_materialCanvas;
	QDockWidget * m_dockMaterial;
	QWidget * m_materialWidget;
	AbstractModelLoader * m_loader;
	Group * m_scene;
};