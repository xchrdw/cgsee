#include <gui/materialwidgetbuilder.h>

#include <qwidget.h>
#include <qboxlayout.h>
#include <qdockwidget.h>
#include <qmainwindow.h>
#include <qdebug.h>
#include <qstring.h>
#include <propertyguizeug/PropertyBrowser.h>

#include <core/material/material.h>
#include <core/scenegraph/group.h>
#include <core/painter/abstractscenepainter.h>
#include <core/assimploader.h>
#include <core/abstractmodelloader.h>
#include <core/glformat.h>
#include <core/camera.h>
#include <core/navigation/arcballnavigation.h>

#include <gui/canvas.h>

#include "../../cgsee/painter.h"
#include "../../cgsee/painter.cpp"

MaterialWidgetBuilder::MaterialWidgetBuilder(QWidget * parent, AbstractModelLoader * loader)
:   m_materialWidget(nullptr)
,   m_dockMaterial(new QDockWidget(QMainWindow::tr("Material")))
,   m_materialCanvas(nullptr)
,   m_propertyMaterialBrowser(nullptr)
,   m_loader(loader)
{
	m_materialWidget = new QWidget(parent);
}

MaterialWidgetBuilder::~MaterialWidgetBuilder()
{
	delete m_materialWidget;
	delete m_dockMaterial;
	delete m_materialCanvas;
	delete m_propertyMaterialBrowser;
	delete m_loader;
}

void MaterialWidgetBuilder::initializeMaterialWidget(const GLFormat & format, Camera * ncamera)
{
	m_dockMaterial->setObjectName("materialWidget");
	
	Material * obj = new Material();

	m_propertyMaterialBrowser = new propertyguizeug::PropertyBrowser(obj);

	m_materialCanvas = new Canvas(format);
	// ToDo: add Vertical splitter layout and handle this 
	m_materialCanvas->setMinimumHeight(128);

	Camera * camera = new Camera("widget");
	camera->selectImplementation("MonoCamera");

	camera->setFovy(45.0f);
	camera->setZNear(1.0f);
	camera->setZFar(20.0f);

	ArcballNavigation * eventhandler = new ArcballNavigation(camera);
	m_materialCanvas->setEventHandler(dynamic_cast<AbstractEventHandler*>(eventhandler));
	eventhandler->setCanvas(m_materialCanvas);

	QVBoxLayout * layout = new QVBoxLayout();
	layout->addWidget(m_materialCanvas);
	layout->addWidget(m_propertyMaterialBrowser);

	m_materialWidget->setLayout(layout);

	Painter * painter = new Painter(camera);

	setPainter(dynamic_cast<AbstractScenePainter *>(painter));

	loadFile("data/shadow_test.obj");
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

void MaterialWidgetBuilder::loadFile(const QString & path)
{
	m_scene = nullptr;
	m_scene = m_loader->importFromFile(path);
	if (!m_scene)
	{
		qDebug("Loading failed", "The loader was not able to load from \n" + path);
	}
	else
	{
		//assignScene(m_scene);
		//dynamic_cast<AbstractNavigation*>(m_materialCanvas->eventHandler())->rescaleScene(m_scene);
		/*if (m_coordinateProvider)
			m_coordinateProvider->assignPass(painter()->getSharedPass());*/
		painter()->assignScene(m_scene);
		dynamic_cast<AbstractNavigation*>(m_materialCanvas->eventHandler())->sceneChanged(m_scene);
		m_materialCanvas->update();
	}
}

void MaterialWidgetBuilder::setPainter(AbstractScenePainter * painter)
{
	if (!m_materialCanvas)
		return;

	m_materialCanvas->setPainter(painter);
}

AbstractScenePainter * MaterialWidgetBuilder::painter()
{
	if (!m_materialCanvas)
		return nullptr;

	// ToDo: painter refactoring
	return dynamic_cast<AbstractScenePainter*>(m_materialCanvas->painter());
}
