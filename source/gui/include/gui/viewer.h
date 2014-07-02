#pragma once

#include <gui/gui_api.h>
#include <core/navigation/navigationhistory.h>

#ifdef WIN32
#include <windows.h>
#else
//from <GL/glew.h>
typedef struct __GLXcontextRec *GLXContext;
#endif

#include <memory>

#include <QMainWindow>
#include <QMap>
#include <QListView>

#include <glm/glm.hpp>



class GLFormat;
class QSettings;
class QStandardItemModel;
class QStandardItem;
class QTreeView;
class QModelIndex;
class QTextEdit;

class Ui_Viewer;
class Canvas;
class AbstractNavigation;
class Camera;
class AbstractScenePainter;

class FileNavigator;
class FileExplorer;
class Viewer;
class AbstractModelLoader;
class Group;
class Node;
class DataBlockRegistry;

class CoordinateProvider;
class AxisAlignedBoundingBox;

namespace propertyguizeug
{
	class PropertyBrowser;
}

class GUI_API Viewer : public QMainWindow
{
    Q_OBJECT

public:
    Viewer(
        std::shared_ptr<DataBlockRegistry> registry = nullptr,
        QWidget * parent = nullptr
    ,   Qt::WindowFlags flags = nullptr);

    void restoreViews( QSettings &s );

    virtual ~Viewer();

    void initialize(const GLFormat & format);

	void setNavigation(AbstractNavigation * navigation);
    AbstractNavigation * navigation();
    
    void setPainter(AbstractScenePainter * painter);
    AbstractScenePainter * painter();

    void setCamera(Camera * camera);
    Camera * camera();

    void setCoordinateProvider(CoordinateProvider * coordinateProvider);
    CoordinateProvider * coordinateProvider();
    
    void keyPressEvent(QKeyEvent * event);
    void keyReleaseEvent (QKeyEvent *event);

    void setFlightManipulator();
    void setTrackballManipulator();
    void setFpsManipulator();
    void uncheckManipulatorActions();
    void uncheckFboActions();

    void selectById(const unsigned int & id);
    void selectNode(Node * node);
    void deselectNode(Node * node);
    void treeToggleSelection(const unsigned int & id);
    void clearSelection();
    void hideById(const unsigned int & id, const bool & hideStatus);
    void updateInfoBox();
    void showMaterial();
    void selectionBBoxChanged();


signals:
    void infoBoxChanged(const QString & info);

public slots:
    void on_flightManipulatorAction_triggered();
    void on_trackballManipulatorAction_triggered();
    void on_fpsManipulatorAction_triggered();

    void on_actionFrontView_triggered();
    void on_actionLeftView_triggered();
    void on_actionBackView_triggered();
    void on_actionRightView_triggered();
    void on_actionTopView_triggered();
    void on_actionBottomView_triggered();
    void on_actionTopRightView_triggered();
    void on_actionBottomLeftView_triggered();
    void on_actionRandomView_triggered();

    void on_actionSave_1_triggered();   
    void on_actionSave_2_triggered();
    void on_actionSave_3_triggered();
    void on_actionSave_4_triggered();
    void on_actionLoad_1_triggered();
    void on_actionLoad_2_triggered();
    void on_actionLoad_3_triggered();
    void on_actionLoad_4_triggered();

    void on_actionHistoryUndo_triggered();
    void on_actionHistoryRedo_triggered();

    void on_loadFile(const QString & path);

protected slots:
    void on_captureAsImageAction_triggered();
    void on_captureAsImageAdvancedAction_triggered();
    void on_enableCullingAction_triggered();

    void on_standardCameraAction_triggered();
    void on_parallelRedCyanStereoCameraAction_triggered();
    void on_convergentRedCyanStereoCameraAction_triggered();
    void on_oculusRiftStereoCameraAction_triggered();

    void on_renderingRasterizerAction_triggered();
    void on_renderingPathtracerAction_triggered();

    void on_reloadAllShadersAction_triggered();
    void on_phongShadingAction_triggered();
    void on_gouraudShadingAction_triggered();
    void on_flatShadingAction_triggered();
    void on_goochShadingAction_triggered();
    void on_wireframeShadingAction_triggered();
    void on_solidWireframeShadingAction_triggered();
    void on_primitiveWireframeShadingAction_triggered();
    void on_normalsAction_triggered();
    void on_colorRenderingAction_triggered();
    void on_shadowMappingAction_triggered();
    void on_shadowBlurAction_triggered();
    void on_ssaoAction_triggered();
    void on_ssaoBlurAction_triggered();
   
    void on_fboColorAction_triggered();
    void on_fboNormalzAction_triggered();
    void on_fboShadowsAction_triggered();
    void on_fboSSAOAction_triggered();
    void on_fboShadowMapAction_triggered();
    void on_fboColorIdAction_triggered();

    void on_openFileDialogAction_triggered();
    void on_quitAction_triggered();
    
    void on_toggleNavigator_triggered();
    void on_toggleExplorer_triggered();
    void on_toggleNavigationHistory_triggered();
    //void on_togglePropertyDemo_triggered(); //TODO: Check if neccessary.
    void on_toggleFullscreen_triggered();

    void on_mouseMoveEventTriggered(int triggered);
    void on_mouseReleaseEventSignal(QMouseEvent * event);

    void on_m_sceneHierarchyTree_clicked(const QModelIndex & index);
    void on_m_sceneHierarchy_itemChanged(QStandardItem * item);

    void on_m_historyList_clicked(const QModelIndex & index);

protected:

    void initializeExplorer();
    void initializeSceneTree();
    void initializeMaterial();    
    void initializeNavigationHistory();
    void initializeDockWidgets(QDockWidget * dockWidget,
    QWidget * widget, Qt::DockWidgetArea area);
    void createSceneHierarchy(QStandardItemModel * model, Node * parentNode);
    void fillSceneHierarchy(Node * node, QStandardItem * parent);
    void assignScene(Group * rootNode);
    void updateHistoryList();

#ifdef WIN32
    const HGLRC createQtContext(const GLFormat & format);
    static const HGLRC currentContextHandle();
#elif __APPLE__
    void * createQtContext(const GLFormat & format);
    static void * currentContextHandle();
#else
    const GLXContext createQtContext(const GLFormat & format);
    static const GLXContext currentContextHandle();
#endif

    void saveView(int i);
    void loadView(int i);

protected:
    const std::unique_ptr<Ui_Viewer> m_ui;
    bool m_visibleDockNavigator;
    bool m_visibleDockExplorer;
    bool m_isFullscreen;
    
    void updateCameraSelection(QString cameraName) const;
    void updateRenderingSelection(QString rendering) const;

    Canvas * m_qtCanvas;
    AbstractNavigation * m_navigation;

    Camera * m_camera;
    QVector<glm::mat4> m_savedViews;

    QDockWidget * m_dockNavigator;
    QDockWidget * m_dockExplorer;
    QDockWidget * m_dockScene;
    QDockWidget * m_dockMaterial;
    QDockWidget * m_dockNavigationHistory;

    Canvas * m_materialCanvas;
	propertyguizeug::PropertyBrowser * m_propertyMaterialBrowser;
	MaterialWidgetBuilder * m_materialWidgetBuilder;

    FileNavigator * m_navigator;
    FileExplorer * m_explorer;
    QStandardItemModel * m_sceneHierarchy;
    QTreeView * m_sceneHierarchyTree;
    AbstractModelLoader * m_loader;
    QListView * m_historyList;
    NavigationHistory * m_navigationHistory;
    Group * m_scene;

    CoordinateProvider * m_coordinateProvider;
    QMap<unsigned int, Node *> m_selectedNodes;

    AxisAlignedBoundingBox * m_selectionBBox;

    bool m_mouseMoving;
};
