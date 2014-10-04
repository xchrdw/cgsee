#pragma once

#include <core/core_api.h>

#include <glm/gtc/matrix_transform.hpp>

//TODO glow integration #include <globjects/FrameBufferObject.h>

#include <memory>

#include <QString>
#include <QMap>
#include <QList>

#include <core/painter/abstractscenepainter.h>
#include <core/coordinateprovider.h>

class DataBlockRegistry;
class Group;
class ScreenQuad;
class FrameBufferObject;
class AbstractProperty;
class RenderingPass;
class LightSourcePass;
class AbstractRenderStage;
class PipelineBuilder;
class AbstractCamera;

namespace globjects{
    class Program;
    class Texture;
}

class CORE_API PipelinePainter : public AbstractScenePainter, public CoordinateProvider // , public PropertyGroup
{
public:
    PipelinePainter(AbstractCamera * camera);
    PipelinePainter(AbstractCamera * camera, Group * scene);
    virtual ~PipelinePainter();

    virtual bool initialize() override;

    //external events
    virtual void pipelineConfigChanged();
    virtual void sceneChanged() override;
    virtual void cameraChanged() override;
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

    //from CoordinateProvider
    virtual unsigned int getObjectID(unsigned int x, unsigned int y) override;
    virtual glm::dvec3 get3DPoint(unsigned int x, unsigned int y) override;
    virtual glm::ivec2 get2DPoint(unsigned int x, unsigned int y) override;

    void selectionChanged(QMap<unsigned int, Node *> selectedNodes);

    const ScreenQuad * screenQuad();

    void addRenderStage(AbstractRenderStage * renderStage);

    bool isSceneInvalid();
    bool isViewInvalid();

    globjects::Texture * getTexture(QString name);
    bool textureExists(QString name);
    void setTexture(QString name, globjects::Texture * texture);
    bool addTexture(QString name, globjects::Texture * texture);
    void removeTexture(QString name);

protected:
    glm::vec3 unproject(float x, float y, float z);
    void setupPipeline(PipelineBuilder & builder);
    void clearRenderStages();


protected:

    QList<AbstractRenderStage *> m_stages;

    QString m_samplerToDisplay;
    QMap<QString, globjects::Texture*> m_textures;

    ScreenQuad * m_quad;
    globjects::Program * m_flush;

    //for CoordinateProvider
    //globjects::Framebuffer m_coordFBO;
};
