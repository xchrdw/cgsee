#pragma once

#include <GL/glew.h>

#include <hash_map>

#include <QMap>

#include "declspec.h"
#include "camera.h"


class BufferObject;
class Program;
class FrameBufferObject;

class CGSEE_API PathTracer : public Camera
{
public:
    const static QMap<QString, GLuint> textureSlots;

    PathTracer(const QString & name = "unnamed");
    virtual ~PathTracer();

    virtual void draw(
        const Program & program
    ,   FrameBufferObject * target = nullptr) override;
    virtual void draw(
        const Program & program
        ,   const glm::mat4 & transform) override;

protected:
    virtual void prepend(Node * node) override;
    virtual void append(Node * node) override;

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

    void invalidateGeometry();

    bool m_invalidatedGeometry;

    GLuint m_vao;
    BufferObject * m_vertexBO;
    GLuint m_randomVectorTexture;
    BufferObject * m_randomVectors;

    // two Framebuffers for "pingPong" rendering -> reuse result of last previous draw call
    bool m_whichBuffer;
    unsigned int m_frameCounter;
    //GLuint m_testTex;
    GLuint m_accuTexture[2];
    GLuint m_accuFramebuffer;
};