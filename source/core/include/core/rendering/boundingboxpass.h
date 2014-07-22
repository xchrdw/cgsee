//#pragma once

//#include <core/core_api.h>

//#include <GL/glew.h>

//#include <QVector>

//#include <glm/glm.hpp>


//#include <core/rendering/renderingpass.h>

//class BufferObject;

//// provides a program, a fbo and default implementations for resize(), output(), clear() and render()
//class CORE_API BoundingBoxPass : public RenderingPass
//{
//public:
//    BoundingBoxPass(Camera * camera, FrameBufferObject * target);
//    virtual ~BoundingBoxPass(void);

//    virtual void resize(const int width, const int height) override;

//    virtual FrameBufferObject * output() override;
//    virtual void clearFbos() override;
//    virtual void setUniforms() override;

//    virtual void updateVertices(const glm::vec3 & llf, const glm::vec3 & urb);

//    virtual void setTransform(const glm::mat4 & transform);

//protected:
//    virtual void initialize();
//    virtual void render() override;

//    GLuint m_vao;
//    BufferObject * m_vertexBO;
//    FrameBufferObject * m_fbo;

//    QVector<glm::vec3> * m_vertices;
//    bool m_validVertices;

//    Program * m_program;

//    glm::mat4 m_transform;
//};
