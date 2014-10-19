#pragma once

#include <core/core_api.h>

#include <glm/gtc/matrix_transform.hpp>

#include <memory>
#include <string>

#include <QMap>
#include <QList>

#include <globjects/Program.h>
#include <globjects/Texture.h>
#include <globjects/base/ref_ptr.h>

#include <reflectionzeug/PropertyGroup.h>

#include <core/painter/abstractscenepainter.h>
#include <core/coordinateprovider.h>

class DataBlockRegistry;
class Group;
class ScreenQuad;
class AbstractProperty;
class RenderingPass;
class LightSourcePass;
class AbstractRenderStage;
class PipelineBuilder;
class AbstractCamera;


class CORE_API PipelinePainter : public AbstractScenePainter, public CoordinateProvider, public reflectionzeug::PropertyGroup
{
public:
    static const std::string VIEWPORT_UNIFORM;
    static const std::string VIEW_UNIFORM;
    static const std::string VIEWINVERSE_UNIFORM;
    static const std::string PROJECTION_UNIFORM;
    static const std::string PROJECTIONINVERSE_UNIFORM;
    static const std::string VIEWPROJECTION_UNIFORM;
    static const std::string VIEWPROJECTIONINVERSE_UNIFORM;
    static const std::string ZNEAR_UNIFORM;
    static const std::string ZFAR_UNIFORM;
    static const std::string CAMERAPOSITION_UNIFORM;

    PipelinePainter(AbstractCamera * camera);
    PipelinePainter(AbstractCamera * camera, Group * scene);
    virtual ~PipelinePainter();

    virtual bool initialize() override;

    //external events
    virtual void pipelineConfigChanged();
    virtual void sceneChanged() override;
    virtual void cameraChanged() override;
    virtual void onViewChanged() override;
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

    virtual void selectionChanged(QMap<unsigned int, Node *> selectedNodes);

    const ScreenQuad * screenQuad();

    virtual void addRenderStage(AbstractRenderStage * renderStage);

    virtual bool isSceneInvalid();
    virtual bool isViewInvalid();

	virtual globjects::Texture* getTexture(QString name);
    virtual bool textureExists(QString name);
    virtual void setTexture(QString name, globjects::Texture * texture);
    virtual bool addTexture(QString name, globjects::Texture * texture);
    virtual void removeTexture(QString name);

    //camera matrices
    virtual const glm::mat4 & view() const;
    virtual const glm::mat4 & projection() const;
    virtual const glm::mat4 & projectionInverse() const;
    virtual const glm::mat4 & viewProjection() const;
    virtual const glm::mat4 & viewProjectionInverse() const;

    virtual void setView(const glm::mat4 & view);
    virtual void setProjection(const glm::mat4 & projection);

    virtual void setCameraUniforms(globjects::Program & program);

    virtual void clearPipeline();

protected:
    glm::vec3 unproject(float x, float y, float z);

	void clearRenderStages();

    void recalculateViewProjection();

protected:

    QList<AbstractRenderStage *> m_stages;

    QString m_samplerToDisplay;
    QMap<QString, globjects::ref_ptr<globjects::Texture>> m_textures;

    ScreenQuad * m_quad;
    globjects::ref_ptr<globjects::Program> m_flush;
	
	//for CoordinateProvider
    //globjects::FrameBufferObject m_coordFBO;

    glm::mat4 m_view;
    glm::mat4 m_viewInverse;
    glm::mat4 m_projection;
    glm::mat4 m_projectionInverse;
    glm::mat4 m_viewProjection;
    glm::mat4 m_viewProjectionInverse;
    glm::vec2 m_viewport;
    float m_zNear;
    float m_zFar;
    glm::vec3 m_eye;
};
