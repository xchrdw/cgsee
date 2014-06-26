#pragma once

#include <core/core_api.h>

#include <glm/glm.hpp>

#include <QVector>

#include <core/scenegraph/group.h>


class Program;
class ViewFrustum;
class FrameBufferObject;

class Painter;

class RenderingPass;
class RenderTechnique;
class Rasterizer;
class PathTracer;

class CameraImplementation;

class CORE_API Camera : public Group
{
public:
    enum Rendering {
        Rasterization,
        PathTracing,
        invalidRendering = -1
    };

    Camera(const QString & name = "unnamed");
    virtual ~Camera();

    virtual Camera * asCamera();

    // call camera implementations and renderer to create visual output
    void drawScene(const Program & program);
    virtual void draw(const Program & program, const glm::mat4 & transform) override;

    const glm::ivec2 & viewport() const;
    void setViewport(const glm::ivec2 & size);
    void setViewport(
        const int width
    ,   const int height);

    const glm::mat4 & projection();

    const glm::mat4 & view() const;
    void setView(const glm::mat4 & view);

    const float fovy() const;
    void setFovy(const float fovy);

    const float zNear() const;
    void setZNear(const float z);

    const float zFar() const;
    void setZFar(const float z);

    const float aspect() const;

    ViewFrustum *viewFrustum() const;

    // updates camera matrices
    void update();
    void setUniforms(const Program & program);

    //
    glm::vec3 getEye() const;
    glm::vec3 getUp() const;
    glm::vec3 getCenter() const;

public:
    void selectImplementation(const QString name);
    QString selectedImplementation() const;
    CameraImplementation * activeImplementation() const;

    void selectRendering(const Rendering rendering);
    void selectRenderingByName(const QString rendering);
    QString selectedRendering() const;
    static QString renderingAsString(const Rendering rendering);
    static Rendering renderingFromString(const QString rendering);

    int preferredRefreshTimeMSec() const;

    void setPainter(Painter * painter);
    
    void drawWithPostprocessing(FrameBufferObject * target);

protected:
    QVector<CameraImplementation*> m_implementations;
    CameraImplementation * m_activeCamera;

    Painter * m_painter;

    // let implementations start rendering as ofter as needed
    void renderScene(const Program & program, FrameBufferObject * target);
    
    // Create render techniques in constructor.
    // const pointers, because we only need to construct the objects once but need to modify them
    Rasterizer *const m_rasterizer;
    PathTracer *const m_pathTracer;
    RenderTechnique * m_activeRenderTechnique;

protected:
    void invalidate();
    virtual void invalidateChildren() override;

    glm::ivec2 m_viewport;

    glm::mat4 m_view;
    glm::mat4 m_projection;

    ViewFrustum *m_viewFrustum;

    float m_fovy;
    float m_zNear;
    float m_zFar;

    bool m_invalidated;

friend CameraImplementation;
};
