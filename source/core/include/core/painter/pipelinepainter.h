#pragma once

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

class PipelinePainter : public AbstractScenePainter // , public PropertyGroup
{
public:
    typedef GLuint Sampler;

    PipelinePainter(Camera * camera, Group * scene);
    virtual ~PipelinePainter();

    //external events
    virtual void pipelineConfigChanged();
    void sceneChanged();
    void viewChanged();
    virtual void paint() override;
    virtual void resize(const int width, const int height) override;
    virtual void reloadShaders() override;

    /* will become properties:
    virtual void selectCamera(const QString cameraName) override;
    virtual void selectRendering(const QString rendering) override;
    virtual void setShading(char shader) override;
    virtual void setFrameBuffer(int frameBuffer);
    virtual void setEffect( int effect, bool active );
    */

    /* will become camera properties:
    void setStereoCameraSeparation(AbstractProperty & property);
    void setConvergentCameraFocus(AbstractProperty & property);
    */

    const Group * scene();
    const ScreenQuad * screenQuad();
    virtual Camera * camera() override;

    void addRenderStage(RenderStage * renderStage);

    void setScene(Group * scene);
    void setCamera(Camera * camera);

    Sampler getSampler(QString name);
    bool samplerExists(QString name);
    void setSampler(QString name, Sampler sampler);
    bool addSampler(QString name, Sampler sampler);
    void removeSampler(QString name);

protected:

    void setupPipeline(PipelineBuilder & builder);
    void clearRenderStages();

protected:

    QList<RenderStage *> m_stages;

    QString m_samplerToDisplay;
    QMap<QString, Sampler> m_samplers;

    Camera * m_camera;
    Group * m_scene;

    ScreenQuad * m_quad;
};
