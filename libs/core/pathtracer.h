#pragma once

#include <GL/glew.h>

#include <memory>
#include <hash_map>

#include <QMap>

#include "declspec.h"
#include "camera.h"


class BufferObject;
class Program;
class FrameBufferObject;
class PathTracingBVH;
class DataBlockRegistry;

class CGSEE_API PathTracer : public Camera
{
public:
    const static QMap<QString, GLuint> textureSlots;

    PathTracer(std::shared_ptr<DataBlockRegistry> registry, const QString & name = "unnamed");
    virtual ~PathTracer();

    virtual void draw(
        const Program & program,
        const glm::mat4 & transform) override;

protected:
    void initialize(const Program & program);
    void initVertexBuffer(const Program & program);
    void initRandomVectorBuffer(const Program & program);
    void buildBoundingVolumeHierarchy();

    void setUniforms(const Program & program);

    virtual void setViewport(
        const int width
    ,   const int height) override;

    static void pointsOnSphere(std::vector<glm::vec3> & points, const unsigned int minN);
    static const glm::uint splitEdge(
        const glm::uint a
    ,   const glm::uint b
    ,   std::vector<glm::vec3> & points
    ,   std::hash_map<glm::highp_uint, glm::uint> & cache);

    PathTracingBVH *m_bvh;

    virtual void invalidate() override;
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

// override Group children changes, to notice when geometry changes
public:
    virtual void removeFirst() override;
    virtual void removeLast () override;
protected:
    virtual const void remove(Node * node, const bool deleteIfParentsEmpty = true) override;
    virtual void prepend(Node * node) override;
    virtual void append(Node * node) override;
    virtual void insert(const t_children::iterator & before, Node * node);
    std::shared_ptr<DataBlockRegistry> m_registry;
};