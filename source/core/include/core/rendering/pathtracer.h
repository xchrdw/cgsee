#pragma once

#include <core/core_api.h>

#include <GL/glew.h>

#include <memory>
#include <vector>

#include <unordered_map>

#include <QMap>

#include <core/rendering/rendertechnique.h>


class BufferObject;
class Program;
class PathTracingBVH;

class CORE_API PathTracer : public RenderTechnique
{
public:
    const static QMap<QString, GLint> textureSlots;

    PathTracer(Camera & abstraction);
    virtual ~PathTracer();
    
    virtual void renderScene(const Program & program, FrameBufferObject * target = nullptr) override;

    virtual int preferredRefreshTimeMSec() const { return 1; };

    const Program * program() const { return m_program; }

protected:
    void initialize();
    void initVertexBuffer();
    void initRandomVectorBuffer();

    void initSkybox();

    void setUniforms();

    void resizeTextures(const glm::ivec2 & viewport) const;
    
    virtual void onInvalidatedView() override;
    virtual void onInvalidatedViewport(const int width, const int height) override;
    virtual void onInvalidatedChildren() override;

    static void pointsOnSphere(std::vector<glm::vec3> & points, const unsigned int minN);
    static const glm::uint splitEdge(
        const glm::uint a
    ,   const glm::uint b
    ,   std::vector<glm::vec3> & points
    ,   std::unordered_map<glm::highp_uint, glm::uint> & cache);

    Program * m_program;

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
};
