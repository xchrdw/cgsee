#pragma once

#include <GL/glew.h>

#include <memory>

#include <unordered_map>

#include <QMap>

#include "declspec.h"
#include "cameraimplementation.h"


class BufferObject;
class Program;
class FrameBufferObject;
class PathTracingBVH;

class CGSEE_API PathTracer : public CameraImplementation
{
public:
    const static QMap<QString, GLuint> textureSlots;

    PathTracer(Camera & abstraction);
    virtual ~PathTracer();

    virtual void draw(
        const Program & program,
        const glm::mat4 & transform) override;

    virtual const QString implementationName() const override;

protected:
    void initialize(const Program & program);
    void initVertexBuffer(const Program & program);
    void initRandomVectorBuffer(const Program & program);

    void initSkybox();

    void setUniforms(const Program & program);
    
    virtual void onInvalidatedView() override;
    virtual void onInvalidatedViewport(
        const int width
    ,   const int height) override;
    virtual void onInvalidatedChildren() override;

    static void pointsOnSphere(std::vector<glm::vec3> & points, const unsigned int minN);
    static const glm::uint splitEdge(
        const glm::uint a
    ,   const glm::uint b
    ,   std::vector<glm::vec3> & points
    ,   std::unordered_map<glm::highp_uint, glm::uint> & cache);

    PathTracingBVH *m_bvh;

    void invalidateGeometry();
    void invalidateAccumulator();
    bool m_invalidatedGeometry;
    bool m_invalidatedAccu;

    GLuint m_vao;
    BufferObject * m_vertexBO;
    GLuint m_randomVectorTexture;
    BufferObject * m_randomVectors;

    // two Framebuffers for "pingPong" rendering -> reuse result of last previous draw call
    bool m_whichBuffer;
    unsigned int m_frameCounter;
    GLuint m_accuTexture[2];
    GLuint m_accuFramebuffer;
    
    GLuint m_staticCubeMap;

private:
    static const QString m_implementationName;
    static bool isRegistered;
};