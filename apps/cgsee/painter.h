#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include <QString>
#include <QMap>

#include <core/abstractscenepainter.h>




class Camera;
class Group;
class ScreenQuad;
class Program;
class FrameBufferObject;

class Painter : public AbstractScenePainter
{
public:
    Painter(Camera * camera);
    virtual ~Painter();

    virtual void paint();

    virtual void setShading(char shader);
    virtual void setFrameBuffer(int frameBuffer);
    virtual void setEffect( int effect, bool active );


    virtual void resize(
        const int width
    ,   const int height);
protected:
    virtual const bool initialize() override;
    virtual Camera * camera() override;

protected:
    void postShaderRelinked();
    
    void setUniforms();

    void swapBuffers();

    typedef QMap<QString, FrameBufferObject *> t_samplerByName;

    void addShadows( t_samplerByName &sampler );
    void addSSAO( t_samplerByName &sampler );

    static void bindSampler(
        const t_samplerByName & sampler
    ,   const Program & program);

    static void releaseSampler(
        const t_samplerByName & sampler);
    void sceneChanged(Group * scene);
protected:
    ScreenQuad * m_quad;

    Program * m_normalz;
    Program * m_normals;
    Program * m_lightsource;
    Program * m_shadowMapping;
    Program * m_SSAO;
    Program * m_wireframe;
    Program * m_primitiveWireframe;
    Program * m_solidWireframe;
    Program * m_flat;
    Program * m_gouraud;
    Program * m_phong;
    Program * m_gooch;
    Program * m_useProgram;
    FrameBufferObject * m_fboColor;
    FrameBufferObject * m_fboTemp;
    FrameBufferObject * m_fboNormalz;
    FrameBufferObject * m_fboShadowMap;
    FrameBufferObject ** m_fboActiveBuffer;


    glm::vec3 camPos;

    Program * m_flush;

    Camera * m_camera;
    Camera * m_lightcam;
    bool m_useShadows;
    bool m_useSSAO;
};
