
#include <core/bufferobject.h>
#include <core/framebufferobject.h>
#include <core/program.h>
#include <core/fileassociatedshader.h>
#include <core/camera.h>

#include "boundingboxpass.h"


const QString TRANSFORM_UNIFORM("transform");
const QString DEPTH_TEXTURE_UNIFORM("depthTexture");
const QString VIEWPORT_UNIFORM("viewport");


BoundingBoxPass::BoundingBoxPass(Camera * camera, FrameBufferObject * target)
:   RenderingPass(camera)
,   m_vao(-1)
,   m_vertexBO(nullptr)
,   m_fbo(target)
,   m_vertices(new QVector<glm::vec3>)
,   m_validVertices(false)
,   m_program(new Program())
,   m_depthTexture(-1)
{
    m_program->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/shading/bbox.frag"));
    m_program->attach(new FileAssociatedShader(GL_GEOMETRY_SHADER, "data/shading/bbox.geo"));
    m_program->attach(new FileAssociatedShader(GL_VERTEX_SHADER, "data/shading/bbox.vert"));
    setActive(true);
}

BoundingBoxPass::~BoundingBoxPass(void)
{
    if(-1 != m_vao)
    {
        delete m_vertexBO;

        glDeleteVertexArrays(1, &m_vao);
        glError();
    }

    delete m_vertices;
    delete m_program;
}


void BoundingBoxPass::initialize()
{
    // By default, counterclockwise polygons are taken to be front-facing.
    // http://www.opengl.org/sdk/docs/man/xhtml/glFrontFace.xml

    glGenVertexArrays(1, &m_vao);
    glError();
    glBindVertexArray(m_vao);                                                                  
    glError();

    // setup array buffer

    if(!m_vertexBO)
    {
        m_vertexBO = new BufferObject(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
        updateVertices(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
        m_vertexBO->data<GLfloat>((GLfloat*)m_vertices->data(), m_vertices->size() * 3, GL_FLOAT, 3);
    }

    // bind all buffers to their attributes

    m_vertexBO->bind(m_program->attributeLocation("a_vertex"));

    glBindVertexArray(0);
    glError();
}

void BoundingBoxPass::updateVertices(const glm::vec3 & llf, const glm::vec3 & urb)
{
    m_vertices->clear();


    m_vertices->append(llf);
    m_vertices->append(glm::vec3(urb.x, llf.y, llf.z));

    m_vertices->append(glm::vec3(urb.x, llf.y, llf.z));
    m_vertices->append(glm::vec3(urb.x, urb.y, llf.z));

    m_vertices->append(llf);
    m_vertices->append(glm::vec3(llf.x, urb.y, llf.z));

    m_vertices->append(glm::vec3(llf.x, urb.y, llf.z));
    m_vertices->append(glm::vec3(urb.x, urb.y, llf.z));

    m_vertices->append(llf);
    m_vertices->append(glm::vec3(llf.x, llf.y, urb.z));

    m_vertices->append(glm::vec3(urb.x, llf.y, llf.z));
    m_vertices->append(glm::vec3(urb.x, llf.y, urb.z));

    m_vertices->append(glm::vec3(llf.x, urb.y, llf.z));
    m_vertices->append(glm::vec3(llf.x, urb.y, urb.z));

    m_vertices->append(glm::vec3(urb.x, urb.y, llf.z));
    m_vertices->append(urb);

    m_vertices->append(glm::vec3(llf.x, llf.y, urb.z));
    m_vertices->append(glm::vec3(urb.x, llf.y, urb.z));

    m_vertices->append(glm::vec3(llf.x, llf.y, urb.z));
    m_vertices->append(glm::vec3(llf.x, urb.y, urb.z));

    m_vertices->append(glm::vec3(llf.x, urb.y, urb.z));
    m_vertices->append(urb);

    m_vertices->append(glm::vec3(urb.x, llf.y, urb.z));
    m_vertices->append(urb);

    m_validVertices = false;
}

void BoundingBoxPass::render()
{
    if(-1 == m_vao)
        initialize();

    if(m_fbo)
        m_fbo->bind();

    if(!m_validVertices)
    {
        m_vertexBO->data<GLfloat>((GLfloat*)m_vertices->data(), m_vertices->size() * 3, GL_FLOAT, 3);
        m_validVertices = true;
    }

    m_program->use();
    m_program->setUniform(TRANSFORM_UNIFORM, m_camera->transform() * m_transform);
    m_program->setUniform(DEPTH_TEXTURE_UNIFORM, 0);
    m_program->setUniform(VIEWPORT_UNIFORM, m_camera->viewport());

    glActiveTexture(GL_TEXTURE0);
    glError();

    glBindTexture(GL_TEXTURE_2D, m_fbo->depthTexture());
    glError();

    glBindVertexArray(m_vao);                                                                  
    glError();

    // glEnable(GL_DEPTH_TEST);

    glLineWidth(2.0f);
    glDrawArrays(GL_LINES, 0, m_vertices->size());
    glError();

    // glDisable(GL_DEPTH_TEST);

    glBindVertexArray(0);
    glError();

    if(m_fbo)
        m_fbo->release();
}

void BoundingBoxPass::clearFbos()
{

}

FrameBufferObject * BoundingBoxPass::output()
{
    return m_fbo;
}

void BoundingBoxPass::resize(const int width, const int height)
{
    m_fbo->resize(width, height);
}

void BoundingBoxPass::setUniforms()
{
    
}

void BoundingBoxPass::setTransform(const glm::mat4 & transform)
{
    m_transform = transform;
}
