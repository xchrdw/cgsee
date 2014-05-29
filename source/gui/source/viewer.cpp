#include <gui/viewer.h>

#include <cassert>

#include <GL/glew.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifdef __GLEW_H__
#undef __GLEW_H__
#include <QOpenGLContext>
#define __GLEW_H__
#else
#include <QOpenGLContext>
#endif

#ifdef WIN32

#elif __APPLE__

#else
//#include <GL/glx.h>

extern "C" {

extern void (*glXGetProcAddress(const unsigned char *procname))( void );
extern GLXContext glXGetCurrentContext( void );

}
#endif

#include <QSettings>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QDockWidget>
#include <QMenu>
#include <QDir>
#include <QDebug>
#include <QFileSystemModel>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QTreeView>
#include <QTextEdit>
#include <QVBoxLayout>

#include <propertyguizeug/PropertyBrowser.h>

#include "gui/ui_viewer.h"
#include <gui/exampleproperties.h>
#include <gui/canvas.h>
#include <gui/canvasexporter.h>
#include <gui/fileNavigator.h>
#include <gui/fileExplorer.h>

#include "../../cgsee/painter.h" // TODO

#include <core/navigation/abstractnavigation.h>
#include <core/navigation/flightnavigation.h>
#include <core/navigation/fpsnavigation.h>
#include <core/navigation/arcballnavigation.h>

#include <core/painter/abstractscenepainter.h>
#include <core/fileassociatedshader.h>
#include <core/glformat.h>
#include <core/assimploader.h>

#include <core/camera.h>
#include <core/parallelcamera.h>
#include <core/coordinateprovider.h>

#include <core/aabb.h>

#include <core/scenegraph/node.h>
#include <core/scenegraph/group.h>
#include <core/scenegraph/polygonaldrawable.h>
#include <core/scenegraph/scenetraverser.h>
#include <core/scenegraph/defaultdrawmethod.h>
#include <core/scenegraph/highlightingdrawmethod.h>


namespace
{
    const QString SETTINGS_GEOMETRY ("Geometry");
    const QString SETTINGS_STATE    ("State");
    const QString SETTINGS_SAVED_VIEWS    ("SavedViews");
}


Viewer::Viewer(
    std::shared_ptr<DataBlockRegistry> registry,
    QWidget  * parent,
    Qt::WindowFlags flags)

:   QMainWindow(parent, flags)
,   m_ui(new Ui_Viewer)
,   m_qtCanvas(nullptr)
,   m_camera(nullptr)
,   m_savedViews(4)
,   m_isFullscreen(false)
,   m_dockNavigator(new QDockWidget(tr("Navigator")))
,   m_dockExplorer(new QDockWidget(tr("Explorer")))
,   m_dockScene(new QDockWidget(tr("SceneHierarchy")))
,   m_dockPropertyDemo(new QDockWidget(tr("PropertyDemo")))
,   m_navigator(new FileNavigator(m_dockNavigator))
,   m_explorer(new FileExplorer(m_dockExplorer))
,   m_sceneHierarchy(new QStandardItemModel())
,   m_sceneHierarchyTree(new QTreeView())
,   m_coordinateProvider(nullptr)
,   m_selectionBBox(new AxisAlignedBoundingBox)
,   m_loader(new AssimpLoader( registry ))
,   m_mouseMoving(false)
{

    m_ui->setupUi(this);

    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings s;

    restoreGeometry(s.value(SETTINGS_GEOMETRY).toByteArray());
    restoreState(s.value(SETTINGS_STATE).toByteArray());

    restoreViews(s);
    initializeExplorer();
    initializeSceneTree();
    initializePropertyDemo();
};

void Viewer::initializeExplorer()
{
    m_dockNavigator->setObjectName("fileNavigator");
    m_dockExplorer->setObjectName("fileExplorer");
    this->initializeDockWidgets(m_dockNavigator, m_navigator, Qt::LeftDockWidgetArea);
    this->initializeDockWidgets(m_dockExplorer, m_explorer, Qt::BottomDockWidgetArea);

    m_explorer->setAllLoadableTypes(m_loader->allLoadableTypes());

    QObject::connect(
        m_navigator, SIGNAL(clickedDirectory(const QString &)),
        m_explorer, SLOT(setRoot(const QString &)));

    QObject::connect(
        m_explorer, SIGNAL(activatedItem(const QString &)),
        this, SLOT(on_loadFile(const QString &)));

    QObject::connect(
        m_explorer, SIGNAL(activatedDir(const QString &)),
        m_navigator, SLOT(on_activatedDir(const QString &)));

    QObject::connect(
        m_ui->openFileDialogAction, SIGNAL(changed()),
        this, SLOT(on_openFileDialogAction_triggered()));

    m_explorer->emitActivatedItem(m_explorer->model()->index(QDir::currentPath()));

    m_dockExplorer->setMinimumHeight(200);
}

void Viewer::initializeSceneTree()
{
    m_sceneHierarchyTree->setModel(m_sceneHierarchy);
    m_sceneHierarchyTree->setSelectionMode(QAbstractItemView::MultiSelection);
    m_sceneHierarchyTree->setItemsExpandable(false);
    m_sceneHierarchyTree->setRootIsDecorated(false);
    m_dockScene->setObjectName("sceneHierarchy");


    QTextEdit * sceneInfoText = new QTextEdit();

    QVBoxLayout * layout = new QVBoxLayout();
    layout->addWidget(m_sceneHierarchyTree);
    layout->addWidget(sceneInfoText);
    layout->setStretch(0,10);
    layout->setStretch(1,1);

    QWidget * sceneWidget = new QWidget(this);
    sceneWidget->setLayout(layout);

    this->initializeDockWidgets(m_dockScene, sceneWidget, Qt::RightDockWidgetArea);

    sceneInfoText->setReadOnly(true);

    QObject::connect(
        m_sceneHierarchyTree, SIGNAL(clicked(const QModelIndex &)),
        this, SLOT(on_m_sceneHierarchyTree_clicked(const QModelIndex &)));

    QObject::connect(
        m_sceneHierarchy, SIGNAL(itemChanged(QStandardItem *)),
        this, SLOT(on_m_sceneHierarchy_itemChanged(QStandardItem *)));

    QObject::connect(
        this, SIGNAL(infoBoxChanged(const QString &)),
        sceneInfoText, SLOT(setPlainText(const QString &)));
}

void Viewer::initializePropertyDemo()
{
    m_dockPropertyDemo->setObjectName("propertyDemo");

    // Yes, this is a memory leak, because the object is never destroyed.
    // Please remove this test as soon as possible :)
    ExampleProperties * obj = new ExampleProperties();

    propertyguizeug::PropertyBrowser *propertyBrowser = new propertyguizeug::PropertyBrowser(obj);

    QVBoxLayout * layout = new QVBoxLayout();
    layout->addWidget(propertyBrowser);

    QWidget * demoWidget = new QWidget(this);
    demoWidget->setLayout(layout);

    this->initializeDockWidgets(m_dockPropertyDemo, demoWidget, Qt::RightDockWidgetArea);
}

void Viewer::initializeDockWidgets(QDockWidget * dockWidget, QWidget * widget, Qt::DockWidgetArea area)
{
    dockWidget->setWidget(widget);
    this->addDockWidget(area, dockWidget);

#ifdef __APPLE__
    /**
     THIS IS A BUG WORKAROUND
     The bug lies somewhere in Canvas::Canvas().
     When called in Viewer::createQtContext(), the widgets get messed up.
    **/
    static int count = 0;
    dockWidget->setFloating(true);
    dockWidget->setAllowedAreas(Qt::NoDockWidgetArea);

    dockWidget->move(QPoint(20, 40 + count++ * (dockWidget->height() + 35)));
#endif
}

void Viewer::createSceneHierarchy(QStandardItemModel * model, Node * rootNode)
{
    QStandardItem * item = new QStandardItem(rootNode->name());
    item->setData(QVariant(rootNode->name()), Qt::DisplayRole);
    item->setData(QVariant(rootNode->id()), Qt::UserRole + 1);
    item->setCheckable(true);
    item->setCheckState(Qt::Checked);

    model->clear();
    model->appendRow(item);

    fillSceneHierarchy(rootNode, item);
}

void Viewer::fillSceneHierarchy(Node * node, QStandardItem * parent)
{
    int childCount = node->children().size();
    int count = 0;
    for (const auto & child : node->children())
    {
        QStandardItem * item = new QStandardItem(child->name());
        if (child->name() == "")
        {
            item->setData(QVariant(node->name() + QString("_") + QString::number(count)), Qt::DisplayRole);
        }
        else
        {
            item->setData(QVariant(child->name()), Qt::DisplayRole);
        }

        item->setData(QVariant(child->id()), Qt::UserRole + 1);
        item->setEditable(false);
        item->setCheckable(true);
        item->setCheckState(Qt::Checked);

        parent->appendRow(item);
        fillSceneHierarchy(child, item);

        if (childCount == 1 && child->children().size() == 0)
            m_sceneHierarchyTree->setRowHidden(0, parent->index(), true);

        ++count;
    }
}

void Viewer::assignScene(Group * rootNode)
{
    m_selectedNodes.clear();

    SceneTraverser traverser;
    unsigned int count = 0;
    traverser.traverse(*rootNode, [&count] (Node & node)
    {
        node.setId(count++);
        return true;
    });

    createSceneHierarchy(m_sceneHierarchy, rootNode);
    m_sceneHierarchyTree->expandAll();
    this->updateInfoBox();
}

#ifdef WIN32
const HGLRC Viewer::currentContextHandle()
{
    return  wglGetCurrentContext();
#elif __APPLE__
void * Viewer::currentContextHandle()
{
    return nullptr;
#else
const GLXContext Viewer::currentContextHandle()
{
    return glXGetCurrentContext();
#endif
}

#ifdef WIN32
const HGLRC Viewer::createQtContext(const GLFormat & format)
#elif __APPLE__
void * Viewer::createQtContext(const GLFormat & format)
#else
const GLXContext Viewer::createQtContext(const GLFormat & format)
#endif
{
    m_qtCanvas = new Canvas(format, this);
    setCentralWidget(m_qtCanvas);
    if (m_camera != nullptr)
        m_qtCanvas->setRefreshTimeMSec(m_camera->preferredRefreshTimeMSec());

    QGLContext * qContext(const_cast<QGLContext *>(m_qtCanvas->context()));

    if(!qContext)
        qFatal("Creating QtGL-Context failed.");

    qContext->makeCurrent();

    if(QGLContext::currentContext() != qContext)
        qFatal("Making QtGL-Context current failed.");

#ifdef WIN32
    const HGLRC qtContextHandle = currentContextHandle();
#elif __APPLE__
    void * qtContextHandle = currentContextHandle();
#else
    const GLXContext qtContextHandle = currentContextHandle();
#endif

    // NOTE: might work even if no context was returned.
    // This just double checks...

    if(nullptr == qtContextHandle)
        qWarning("Acquiring QtGL-Context handle failed.");

    // canvas verifies the context itself.

    qContext->doneCurrent();

    return qtContextHandle;
}

void Viewer::initialize(const GLFormat & format)
{
    if(!QGLFormat::hasOpenGL())
        qFatal("OpenGL not supported.");

    createQtContext(format);

    QObject::connect(
        m_qtCanvas, SIGNAL(mouseReleaseEventSignal(QMouseEvent *)),
        this, SLOT(on_mouseReleaseEventSignal(QMouseEvent *)));

    QObject::connect(
        m_qtCanvas, SIGNAL(mouseMoveEventTriggered(int)),
        this, SLOT(on_mouseMoveEventTriggered(int)));
}

Viewer::~Viewer()
{
    QSettings s;
    s.setValue(SETTINGS_GEOMETRY, saveGeometry());
    s.setValue(SETTINGS_STATE, saveState());

    delete m_qtCanvas;

    delete m_dockNavigator;
    delete m_dockExplorer;
    delete m_dockScene;
    delete m_sceneHierarchy;
    delete m_loader;
    delete m_coordinateProvider;
    delete m_selectionBBox;
}

void Viewer::setPainter(AbstractScenePainter * painter)
{
    if(!m_qtCanvas)
        return;

    m_qtCanvas->setPainter(painter);
}

AbstractScenePainter * Viewer::painter()
{
    if(!m_qtCanvas)
        return nullptr;

    return m_qtCanvas->painter();
}


void Viewer::on_captureAsImageAction_triggered()
{
    m_dockNavigator->show();
    assert(m_qtCanvas);
    CanvasExporter::save(*m_qtCanvas, this);
}

void Viewer::on_captureAsImageAdvancedAction_triggered()
{
    assert(m_qtCanvas);
    CanvasExporter::save(*m_qtCanvas, this, true);
}

void Viewer::on_reloadAllShadersAction_triggered()
{
    FileAssociatedShader::reloadAll();
    painter()->postShaderRelinked();
    m_qtCanvas->repaint();
}

void Viewer::on_openFileDialogAction_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
        QDir::homePath(), m_loader->namedLoadableTypes().join(";;"),
        0, QFileDialog::HideNameFilterDetails);
    if (fileName.isEmpty())
        return;

    on_loadFile(fileName);
}

void Viewer::on_quitAction_triggered()
{
    QApplication::quit();
}

void Viewer::on_loadFile(const QString & path)
{
    Group * scene = m_loader->importFromFile(path);
    if (!scene)
    {
        QMessageBox::critical(this, "Loading failed", "The loader was not able to load from \n" + path);
    }
    else
    {
        this->assignScene(scene);
        this->m_qtCanvas->navigation()->rescaleScene(scene);
        if (m_coordinateProvider)
            this->m_coordinateProvider->assignPass(this->painter()->getSharedPass());
        this->painter()->assignScene(scene);
        this->m_qtCanvas->navigation()->sceneChanged(scene);
        this->m_qtCanvas->update();
        this->selectionBBoxChanged();
    }
}

void Viewer::on_toggleNavigator_triggered()
{
    bool visible = m_dockNavigator->isVisible();
    m_dockNavigator->setVisible(!visible);
}

void Viewer::on_toggleExplorer_triggered()
{
    bool visible = m_dockExplorer->isVisible();
    m_dockExplorer->setVisible(!visible);
}

void Viewer::updateCameraSelection(QString cameraName) const
{
}

void Viewer::updateRenderingSelection(QString rendering) const
{
    m_qtCanvas->painter()->selectRendering(rendering);
    m_qtCanvas->setRefreshTimeMSec(m_camera->preferredRefreshTimeMSec());
}

void Viewer::on_renderingRasterizerAction_triggered()
{
    updateRenderingSelection("Rasterization");
}

void Viewer::on_renderingPathtracerAction_triggered()
{
    updateRenderingSelection("PathTracing");
}

void Viewer::on_phongShadingAction_triggered()
{
    m_qtCanvas->painter()->setShading('p');
    m_qtCanvas->repaint();
}

void Viewer::on_gouraudShadingAction_triggered()
{
    m_qtCanvas->painter()->setShading('g');
    m_qtCanvas->repaint();
}

void Viewer::on_flatShadingAction_triggered()
{
    m_qtCanvas->painter()->setShading('f');
    m_qtCanvas->repaint();
}

void Viewer::on_goochShadingAction_triggered()
{
    m_qtCanvas->painter()->setShading('o');
    m_qtCanvas->repaint();
}

void Viewer::on_wireframeShadingAction_triggered()
{
    m_qtCanvas->painter()->setShading('w');
    m_qtCanvas->repaint();
}

void Viewer::on_solidWireframeShadingAction_triggered()
{
    m_qtCanvas->painter()->setShading('s');
    m_qtCanvas->repaint();
}

void Viewer::on_primitiveWireframeShadingAction_triggered()
{
    m_qtCanvas->painter()->setShading('r');
    m_qtCanvas->repaint();
}

void Viewer::on_normalsAction_triggered()
{
    m_qtCanvas->painter()->setShading('n');
    m_qtCanvas->repaint();
}

void Viewer::on_colorRenderingAction_triggered()
{
    m_qtCanvas->painter()->setEffect(1, m_ui->colorRenderingAction->isChecked());
    m_qtCanvas->repaint();
}

void Viewer::on_shadowMappingAction_triggered()
{
    m_qtCanvas->painter()->setEffect(2, m_ui->shadowMappingAction->isChecked());
    m_qtCanvas->repaint();
}

void Viewer::on_shadowBlurAction_triggered()
{
    m_qtCanvas->painter()->setEffect(3, m_ui->shadowBlurAction->isChecked());
    m_qtCanvas->repaint();
}

void Viewer::on_ssaoAction_triggered()
{
    m_qtCanvas->painter()->setEffect(4, m_ui->ssaoAction->isChecked());
    m_qtCanvas->repaint();
}

void Viewer::on_ssaoBlurAction_triggered()
{
    m_qtCanvas->painter()->setEffect(5, m_ui->ssaoBlurAction->isChecked());
    m_qtCanvas->repaint();
}

void Viewer::uncheckFboActions()
{
    m_ui->fboColorAction->setChecked(false);
    m_ui->fboNormalzAction->setChecked(false);
    m_ui->fboShadowMapAction->setChecked(false);
    m_ui->fboShadowsAction->setChecked(false);
    m_ui->fboSSAOAction->setChecked(false);
    m_ui->fboColorIdAction->setChecked(false);
    m_ui->fboTempBufferAction->setChecked(false);
}

void Viewer::on_fboColorAction_triggered()
{
    uncheckFboActions();
    m_ui->fboColorAction->setChecked(true);
    m_qtCanvas->painter()->setFrameBuffer(1);
    m_qtCanvas->repaint();
}

void Viewer::on_fboNormalzAction_triggered()
{
    uncheckFboActions();
    m_ui->fboNormalzAction->setChecked(true);
    m_qtCanvas->painter()->setFrameBuffer(2);
    m_qtCanvas->repaint();
}

void Viewer::on_fboShadowsAction_triggered()
{
    uncheckFboActions();
    m_ui->fboShadowsAction->setChecked(true);
    m_qtCanvas->painter()->setFrameBuffer(3);
    m_qtCanvas->repaint();
}

void Viewer::on_fboShadowMapAction_triggered()
{
    uncheckFboActions();
    m_ui->fboShadowMapAction->setChecked(true);
    m_qtCanvas->painter()->setFrameBuffer(4);
    m_qtCanvas->repaint();
}

void Viewer::on_fboSSAOAction_triggered()
{
    uncheckFboActions();
    m_ui->fboSSAOAction->setChecked(true);
    m_qtCanvas->painter()->setFrameBuffer(5);
    m_qtCanvas->repaint();
}

void Viewer::on_fboColorIdAction_triggered()
{
    uncheckFboActions();
    m_ui->fboColorIdAction->setChecked(true);
    m_qtCanvas->painter()->setFrameBuffer(6);
    m_qtCanvas->repaint();
}

void Viewer::on_standardCameraAction_triggered()
{
    glm::ivec2 tempViewport = m_camera->viewport();
    m_camera->selectImplementation("MonoCamera");

    m_qtCanvas->resize(tempViewport.x-1,tempViewport.y-1);

    qDebug("Standard Mono Camera");
}

void Viewer::on_parallelRedCyanStereoCameraAction_triggered()
{
    glm::ivec2 tempViewport = m_camera->viewport();
    m_camera->selectImplementation("ParallelCamera");
    ParallelCamera * parallelCam = dynamic_cast<ParallelCamera*>(m_camera->activeImplementation());
    if (parallelCam != nullptr)
        parallelCam->deactivateOculusRift();
    else
        qDebug() << "Expected ParallelCamera as active implementation but was"
                << m_camera->selectedImplementation();

    m_qtCanvas->resize(tempViewport.x-1,tempViewport.y-1);

    qDebug("(Parallel) Red Cyan Stereo Camera");
}

void Viewer::on_convergentRedCyanStereoCameraAction_triggered()
{
    glm::ivec2 tempViewport = m_camera->viewport();
    m_camera->selectImplementation("ConvergentCamera");

    m_qtCanvas->resize(tempViewport.x-1,tempViewport.y-1);

    qDebug("(Convergent) Red Cyan Stereo Camera");
}

void Viewer::on_oculusRiftStereoCameraAction_triggered()
{
    glm::ivec2 tempViewport = m_camera->viewport();
    m_camera->selectImplementation("ParallelCamera");

    ParallelCamera * parallelCam = dynamic_cast<ParallelCamera*>(m_camera->activeImplementation());
    if (parallelCam != nullptr)
        parallelCam->activateOculusRift();
    else
        qDebug() << "Expected ParallelCamera as active implementation but was"
                << m_camera->selectedImplementation();

    m_qtCanvas->resize(tempViewport.x-1,tempViewport.y-1);

    qDebug() << "Stereo Camera for Oculus Rift";

    ((QWidget*)m_qtCanvas->parent())->showFullScreen();
    m_visibleDockNavigator = m_dockNavigator->isVisible();
    m_dockNavigator->setVisible(false);
    m_visibleDockExplorer = m_dockExplorer->isVisible();
    m_dockExplorer->setVisible(false);
    menuBar()->setVisible(false);
    statusBar()->setVisible(false);
    m_isFullscreen=true;
}

void Viewer::on_toggleFullscreen_triggered()
{
    if(!m_isFullscreen)
    {
        ((QWidget*)m_qtCanvas->parent())->showFullScreen();
        m_visibleDockNavigator = m_dockNavigator->isVisible();
        m_dockNavigator->setVisible(false);
        m_visibleDockExplorer = m_dockExplorer->isVisible();
        m_dockExplorer->setVisible(false);
        menuBar()->setVisible(false);
        menuBar()->setShortcutEnabled(Qt::Key_Escape,true);
        statusBar()->setVisible(false);
        m_isFullscreen=true;
    }
    else
    {
        ((QWidget*)m_qtCanvas->parent())->showNormal();
        m_dockExplorer->setVisible(m_visibleDockExplorer);
        m_dockNavigator->setVisible(m_visibleDockNavigator);
        menuBar()->setVisible(true);
        statusBar()->setVisible(true);
        m_isFullscreen=false;
    }
}

void Viewer::setNavigation(AbstractNavigation * navigation)
{
    m_qtCanvas->setNavigation(navigation);
    this->setFocus();
}

AbstractNavigation * Viewer::navigation()
{
    if(!m_qtCanvas)
        return nullptr;
    return m_qtCanvas->navigation();
}

void Viewer::setCamera(Camera * camera )
{
    m_camera = camera;
    if ((m_camera != nullptr) && (m_qtCanvas != nullptr))
        m_qtCanvas->setRefreshTimeMSec(m_camera->preferredRefreshTimeMSec());
}

Camera * Viewer::camera()
{
    return m_camera;
}

void Viewer::setCoordinateProvider(CoordinateProvider * coordinateProvider )
{
    if (coordinateProvider)
    {
        delete m_coordinateProvider;
        m_coordinateProvider = coordinateProvider;
    }
}

CoordinateProvider * Viewer::coordinateProvider()
{
    return m_coordinateProvider;
}

void Viewer::keyPressEvent(QKeyEvent * event)
{
    if(!event->isAutoRepeat())
    {
        navigation()->keyPressEvent(event);
    }
}

void Viewer::keyReleaseEvent( QKeyEvent *event )
{
    if(!event->isAutoRepeat())
    {
        navigation()->keyReleaseEvent(event);
    }
}

void Viewer::on_flightManipulatorAction_triggered()
{
    setNavigation(new FlightNavigation(m_camera));
    uncheckManipulatorActions();
    m_ui->flightManipulatorAction->setChecked(true);
    qDebug("Flight navigation, use WASD and arrow keys");
}

void Viewer::on_trackballManipulatorAction_triggered()
{
    setNavigation(new ArcballNavigation(m_camera));
    uncheckManipulatorActions();
    m_ui->trackballManipulatorAction->setChecked(true);
    qDebug("Arcball navigation, use left and right mouse buttons");
}

void Viewer::on_fpsManipulatorAction_triggered()
{
    setNavigation(new FpsNavigation(m_camera));
    uncheckManipulatorActions();
    m_ui->fpsManipulatorAction->setChecked(true);
    qDebug("FPS Navigation, use mouse and WASD");
}

void Viewer::uncheckManipulatorActions()
{
    m_ui->flightManipulatorAction->setChecked(false);
    m_ui->fpsManipulatorAction->setChecked(false);
    m_ui->trackballManipulatorAction->setChecked(false);
}


// helper to restore mat4
glm::mat4 string2mat(QString s)
{
    glm::mat4 mat;
    QStringList list = s.split(';');
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            mat[j][i] = list.at(j + 4*i).toFloat();
        }
    }
    return mat;
}

// helper to save mat4
QString mat2string(glm::mat4 mat)
{
    QString s("");
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            s += QString::number(mat[j][i]);
            s += ';';
        }
    }
    return s;
}

void Viewer::restoreViews( QSettings &s )
{
    s.beginGroup(SETTINGS_SAVED_VIEWS);
    for (int i = 0; i < m_savedViews.size(); i++)
    {
        QString name = "view_" + QString::number(i+1);
        QString str = s.value(name, "").value<QString>();
        if (str.size() > 0)
        {
            m_savedViews[i] = string2mat(str);
        } else
        {
            m_savedViews[i] = glm::mat4(0);
        }

    }
    s.endGroup();
}

void Viewer::saveView(int i)
{
    m_savedViews[i] = navigation()->viewMatrix();
    QSettings s;
    s.beginGroup(SETTINGS_SAVED_VIEWS);
    s.setValue("view_" + QString::number(i+1), mat2string(navigation()->viewMatrix()));
    s.endGroup();
}

void Viewer::loadView(int i)
{
    if (m_savedViews[i] == glm::mat4(0))
    {
        navigation()->loadView(navigation()->defaultView());
    }
    else {
        navigation()->loadView(m_savedViews[i]);
    }
}

void Viewer::on_actionFrontView_triggered()
{
    navigation()->loadView(navigation()->frontview());
}

void Viewer::on_actionLeftView_triggered()
{
    navigation()->loadView(navigation()->leftview());
}

void Viewer::on_actionBackView_triggered()
{
    navigation()->loadView(navigation()->backview());
}

void Viewer::on_actionRightView_triggered()
{
    navigation()->loadView(navigation()->rightview());
}

void Viewer::on_actionTopView_triggered()
{
    navigation()->loadView(navigation()->topview());
}

void Viewer::on_actionBottomView_triggered()
{
    navigation()->loadView(navigation()->bottomview());
}

void Viewer::on_actionTopRightView_triggered()
{
    navigation()->loadView(navigation()->topRightView());
}

void Viewer::on_actionBottomLeftView_triggered()
{
    navigation()->loadView(navigation()->bottomLeftView());
}

void Viewer::on_actionRandomView_triggered()
{
    navigation()->loadView(navigation()->randomView());
}


void Viewer::on_actionLoad_1_triggered() { loadView(0); }
void Viewer::on_actionLoad_2_triggered() { loadView(1); }
void Viewer::on_actionLoad_3_triggered() { loadView(2); }
void Viewer::on_actionLoad_4_triggered() { loadView(3); }

void Viewer::on_actionSave_1_triggered() { saveView(0); }
void Viewer::on_actionSave_2_triggered() { saveView(1); }
void Viewer::on_actionSave_3_triggered() { saveView(2); }
void Viewer::on_actionSave_4_triggered() { saveView(3); }

void Viewer::on_actionHistoryUndo_triggered() { m_qtCanvas->viewhistory()->undo(); }
void Viewer::on_actionHistoryRedo_triggered() { m_qtCanvas->viewhistory()->redo(); }


void Viewer::on_mouseMoveEventTriggered(int triggered)
{
    m_mouseMoving = triggered;
}

void Viewer::on_mouseReleaseEventSignal(QMouseEvent * event)
{
    static const unsigned int BACKGROUND_ID = 4244897280;

    if (m_mouseMoving)
    {
        m_mouseMoving = false;
        return;
    }

    if (m_coordinateProvider && event->button() == Qt::LeftButton)
    {
        unsigned int id = m_coordinateProvider->objID(event->x(), event->y());

        if (event->modifiers() != Qt::CTRL)
            this->clearSelection();

        if (id < BACKGROUND_ID)
        {
            this->selectById(id);
            this->treeToggleSelection(id);
        }
    }

    if (event->button() == Qt::RightButton)
        this->clearSelection();
}

void Viewer::selectById(const unsigned int & id)
{
    SceneTraverser traverser;
    Node * result = nullptr;
    traverser.traverse(*m_camera, [&result, &id](Node & node)
    {
        if(node.id() == id)
        {
            result = &node;
            return false;
        }
        return true;
    });

    if (result)
    {
        Node * parent = *result->parents().begin();
        int siblings = parent->children().size() - 1;
        if (m_selectedNodes.contains(id))
        {
            this->deselectNode(result);
            if (!siblings)
            {
                this->deselectNode(parent);
                this->treeToggleSelection(parent->id());
            }
        }
        else
        {
            this->selectNode(result);
            if (!siblings)
            {
                this->selectNode(parent);
                this->treeToggleSelection(parent->id());
            }
        }
    }
    this->updateInfoBox();
}

void Viewer::selectNode(Node * node)
{
    static std::shared_ptr<DrawMethod> highlightingDrawmethod = std::make_shared<HighlightingDrawMethod>();

    m_selectedNodes.insert(node->id(), node);
    node->setSelected(true);
    if (PolygonalDrawable * drawable = dynamic_cast<PolygonalDrawable*>(node))
        drawable->setDrawMethod(highlightingDrawmethod);

    this->selectionBBoxChanged();
    this->m_qtCanvas->update();

    for ( auto child : node->children() )
    {
        if (!m_selectedNodes.contains(child->id()))
        {
            this->selectNode(child);
            this->treeToggleSelection(child->id());
        }
    }
}

void Viewer::deselectNode(Node * node)
{
    static std::shared_ptr<DrawMethod> defaultDrawmethod = std::make_shared<DefaultDrawMethod>();

    m_selectedNodes.erase(m_selectedNodes.find(node->id()));
    node->setSelected(false);
    if (PolygonalDrawable * drawable = dynamic_cast<PolygonalDrawable*>(node))
        drawable->setDrawMethod(defaultDrawmethod);

    this->selectionBBoxChanged();
    this->m_qtCanvas->update();

    for ( auto child : node->children() )
    {
        if (m_selectedNodes.contains(child->id()))
        {
            this->deselectNode(child);
            this->treeToggleSelection(child->id());
        }
    }
}

void Viewer::treeToggleSelection(const unsigned int & id)
{
    QModelIndexList list = m_sceneHierarchy->match(
        m_sceneHierarchy->indexFromItem((m_sceneHierarchy->item(0,0))),
        Qt::UserRole + 1,
        QVariant(id),
        2,
        Qt::MatchRecursive);

    if (list.isEmpty())
        return;

    m_sceneHierarchyTree->selectionModel()->select(list.first(), QItemSelectionModel::Toggle);
}

void Viewer::clearSelection()
{
    static std::shared_ptr<DrawMethod> defaultDrawmethod = std::make_shared<DefaultDrawMethod>();

    for( auto node : m_selectedNodes )
        {
            node->setSelected(false);
            if (PolygonalDrawable * drawable = dynamic_cast<PolygonalDrawable*>(node))
                drawable->setDrawMethod(defaultDrawmethod);
        }

        m_selectedNodes.clear();
        this->selectionBBoxChanged();
        this->m_qtCanvas->update();

        m_sceneHierarchyTree->clearSelection();
}

void Viewer::hideById(const unsigned int & id, const bool & hideStatus)
{
    SceneTraverser traverser;
    Node * result = nullptr;
    traverser.traverse(*m_camera, [&result, &id](Node & node)
    {
        if( node.id() == id)
        {
            result = &node;
            return false;
        }
        return true;
    });

    if (result)
    {
        result->setHidden(hideStatus);
    }
}

void Viewer::on_m_sceneHierarchyTree_clicked(const QModelIndex & index)
{
    if (QApplication::keyboardModifiers() != Qt::CTRL)
    {
        this->clearSelection();
        this->treeToggleSelection(index.data(Qt::UserRole + 1).toUInt());
    }
    this->selectById(index.data(Qt::UserRole + 1).toUInt());
}

void Viewer::on_m_sceneHierarchy_itemChanged(QStandardItem * item)
{
    this->hideById(item->data(Qt::UserRole + 1).toUInt(), item->checkState() == Qt::Unchecked);
}

void Viewer::updateInfoBox()
{
    int vertices = 0;

    for ( auto node : m_selectedNodes )
    {
        if (PolygonalDrawable * drawable = dynamic_cast<PolygonalDrawable*>(node))
            vertices += drawable->numVertices();
    }

    QString info = "(Selected objects)\n\nVertices: " + QString::number(vertices);

    emit infoBoxChanged(info);
}

void Viewer::selectionBBoxChanged()
{
    m_selectionBBox->invalidate();
    for ( auto node : m_selectedNodes )
    {
        m_selectionBBox->extend(node->boundingBox());
    }

    Painter * painter = static_cast<Painter*>(this->painter());
    painter->setBoundingBox(m_selectionBBox->llf(), m_selectionBBox->urb(), this->m_qtCanvas->navigation()->sceneTransform());
}
