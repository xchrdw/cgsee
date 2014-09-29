
#include <core/rendering/renderstage.h>

#include <glm/mat4x4.hpp>

#include <core/program.h>
#include <core/scenegraph/scenetraverser.h>
#include <core/scenegraph/drawvisitor.h>
#include <core/camera/abstractcamera.h>
#include <core/fileassociatedshader.h>

RenderStage::RenderStage(PipelinePainter & painter, const QString & normalzBufferName)
    : AbstractSceneRenderStage(painter)
    , m_normalz(new TextureObject(gl::GLenum::GL_RGBA32F, gl::GLenum::GL_RGBA, gl::GLenum::GL_FLOAT))
    , m_colorId(nullptr)
    , m_depth(new RenderBufferObject(gl::GLenum::GL_DEPTH24_STENCIL8))
{

    m_painter.addTexture(normalzBufferName, m_normalz);

    m_program->attach(new FileAssociatedShader(gl::GLenum::GL_FRAGMENT_SHADER, "data/normalz.frag"));
    m_program->attach(new FileAssociatedShader(gl::GLenum::GL_FRAGMENT_SHADER, "data/depth_util.frag"));
    m_program->attach(new FileAssociatedShader(gl::GLenum::GL_VERTEX_SHADER, "data/normalz.vert"));
}

RenderStage::~RenderStage(void)
{

}

void RenderStage::render()
{
    m_painter.setCameraUniforms(*m_program);
    bindFBO();
    gl::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawScene(m_painter.transform(), m_program);
    releaseFBO();
}
void RenderStage::reloadShaders()
{
    //TODO deprecated (glow)
}

void RenderStage::resize(const int width, const int height)
{
    m_normalz->resize(width, height);
    m_depth->resize(width, height);
    bindFBO();
    m_normalz->attachTo(gl::GLenum::GL_COLOR_ATTACHMENT0);
    m_depth->attachTo(gl::GLenum::GL_DEPTH_STENCIL_ATTACHMENT);
    releaseFBO();
}
