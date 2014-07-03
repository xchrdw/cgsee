#pragma once

#include <core/core_api.h>
#include <core/gpuquery.h>

#include <glm/gtc/matrix_transform.hpp>

#include <memory>

#include <QString>
#include <QMap>
#include <QList>

#include <core/painter/abstractscenepainter.h>

class DataBlockRegistry;
class Camera;
class Group;
class ScreenQuad;
class Program;
class FrameBufferObject;
class AbstractProperty;
class RenderingPass;
class LightSourcePass;
class RenderStage;
class PipelineBuilder;
class TextureObject;

class CORE_API PipelinePainter : public AbstractScenePainter // , public PropertyGroup
{
public:
    PipelinePainter(Camera * camera);
    PipelinePainter(Camera * camera, Group * scene);
    virtual ~PipelinePainter();

    virtual bool initialize() override;

    //external events
    virtual void pipelineConfigChanged();
    void sceneChanged();
    void viewChanged();
    virtual void paint() override;
    virtual void resize(const int width, const int height) override;
    virtual void reloadShaders() override;

    //TODO remove as soon as possible
    virtual void selectCamera(const QString cameraName) override;
    virtual void selectRendering(const QString rendering) override;
    virtual void setShading(char shader) override;
    virtual void setFrameBuffer(int frameBuffer);
    virtual void setEffect( int effect, bool active );
    void setStereoCameraSeparation(AbstractProperty & property);
    void setConvergentCameraFocus(AbstractProperty & property);
    virtual void setViewFrustumCulling(bool viewFrustumCullingEnabled) override;
    virtual bool isViewFrustumCullingEnabled() override;
    virtual void postShaderRelinked() override;
    virtual RenderingPass *  getSharedPass() override;
    //TODO remove as soon as possible </end>

    Group * scene();
    const ScreenQuad * screenQuad();
    virtual Camera * camera() override;

    void addRenderStage(RenderStage * renderStage);

    void setScene(Group * scene);
    void setCamera(Camera * camera);

    bool isSceneInvalid();
    bool isViewInvalid();

    TextureObject * getTexture(QString name);
    bool textureExists(QString name);
    void setTexture(QString name, TextureObject * texture);
    bool addTexture(QString name, TextureObject * texture);
    void removeTexture(QString name);

protected:

    void setupPipeline(PipelineBuilder & builder);
    void clearRenderStages();


protected:

    QList<RenderStage *> m_stages;

    QString m_samplerToDisplay;
    QMap<QString, TextureObject*> m_textures;

    Camera * m_camera;
    Group * m_scene;

    ScreenQuad * m_quad;
    Program * m_flush;
};
