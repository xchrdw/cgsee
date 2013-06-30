#pragma once

#include <GL/glew.h>

#include <hash_map>

#include "declspec.h"
#include "camera.h"


class BufferObject;
class Program;
class FrameBufferObject;

class CGSEE_API PathTracer : public Camera
{
public:
    PathTracer(const QString & name = "unnamed");
    virtual ~PathTracer();

    virtual void draw(
        const Program & program
    ,   FrameBufferObject * target = nullptr) override;
    virtual void draw(
        const Program & program
        ,   const glm::mat4 & transform) override;
    
    void append(Group * group);

protected:
    void initialize(const Program & program);
    void initVertexBuffer(const Program & program);
    void initRandomVectorBuffer(const Program & program);
    void buildBoundingVolumeHierarchy();

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
};