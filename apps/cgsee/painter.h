#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include <memory>

#include <QString>
#include <QMap>

#include <core/painter/abstractscenepainter.h>

class DataBlockRegistry;
class Camera;
class Group;
class ScreenQuad;
class PathTracer;
class Program;
class FrameBufferObject;
class AbstractProperty;
class RenderingPass;
class LightSourcePass;

class Painter : public AbstractScenePainter
{
public:
    Painter(Camera * camera);
    virtual ~Painter();

    virtual void paint();

    virtual void selectCamera(QString cameraName) override;
    virtual void setShading(char shader) override;
    virtual void setFrameBuffer(int frameBuffer);
    virtual void setEffect( int effect, bool active );

    virtual void resize(const int width, const int height);


    void helloWord(AbstractProperty & property);
    void setStereoCameraSeparation(AbstractProperty & property);
    void setConvergentCameraFocus(AbstractProperty & property);
    virtual void postShaderRelinked() override;

    virtual void setBoundingBox(const glm::vec3 & llf, const glm::vec3 & urb, const glm::mat4 & transform);

    virtual RenderingPass * getSharedPass();
    
protected:
    virtual const bool initialize() override;
    virtual Camera * camera() override;

protected:
    void setUniforms();

    typedef QMap<QString, FrameBufferObject *> t_samplerByName;

    void drawScene(Camera * camera, Program * program, FrameBufferObject * fbo);

    static void bindSampler(const t_samplerByName & sampler, const Program & program);

    static void releaseSampler(const t_samplerByName & sampler);
    void sceneChanged(Group * scene);

protected:  
    ScreenQuad * m_quad;
    Program * m_normals;
    Program * m_wireframe;
    Program * m_primitiveWireframe;
    Program * m_solidWireframe;
    Program * m_flat;
    Program * m_gouraud;
    Program * m_phong;
    Program * m_gooch;
    Program * m_pathTracing;
    Program * m_useProgram;
    Program * m_lastUsedProgram;
    Program * m_flush;
    FrameBufferObject * m_fboColor;
    FrameBufferObject * m_fboTemp;
    FrameBufferObject * m_fboActiveBuffer;

    RenderingPass * m_normalz;
    LightSourcePass * m_lightsource;
    RenderingPass * m_shadows;
    RenderingPass * m_shadowBlur;
    RenderingPass * m_ssao;
    RenderingPass * m_ssaoBlur;
    RenderingPass * m_colorId;
    RenderingPass * m_boundingBox;
    QList<RenderingPass*> m_passes;

    Camera * m_camera;
    bool m_useColor;

};
