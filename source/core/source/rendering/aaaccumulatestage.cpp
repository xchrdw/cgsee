
#include <core/rendering/aaaccumulatestage.h>

#include <glbinding/glbinding.h>

#include <core/painter/pipelinepainter.h>
#include <core/fileassociatedshader.h>
#include <core/textureobject.h>
#include <core/program.h>
#include <core/screenquad.h>

AAAccumulateStage::AAAccumulateStage(PipelinePainter & painter, const QString & inputBufferName, const QString & outputBufferName)
    : AbstractPostProcessingStage(painter)
    , m_outputBufferName(outputBufferName)
    , m_targetBuffer(false)
    , m_inputBuffer(painter.getTexture(inputBufferName))
{

    m_accumulateBuffer[0] = new TextureObject(gl::GLenum::GL_RGBA32F, gl::GLenum::GL_RGBA, gl::GLenum::GL_FLOAT);
    m_accumulateBuffer[1] = new TextureObject(gl::GLenum::GL_RGBA32F, gl::GLenum::GL_RGBA, gl::GLenum::GL_FLOAT);

    m_painter.addTexture(m_outputBufferName, m_accumulateBuffer[m_targetBuffer? 0 : 1]);

    m_program->attach(new FileAssociatedShader(gl::GLenum::GL_FRAGMENT_SHADER, "data/aaaccumulate.frag"));
    m_program->attach(new FileAssociatedShader(gl::GLenum::GL_VERTEX_SHADER, "data/screenquad.vert"));
}

AAAccumulateStage::~AAAccumulateStage(void)
{
    delete m_accumulateBuffer[0];
    delete m_accumulateBuffer[1];
}

void AAAccumulateStage::reloadShaders()
{

}

void AAAccumulateStage::resize(const int width, const int height)
{
    m_accumulateBuffer[0]->resize(width, height);
    m_accumulateBuffer[1]->resize(width, height);
}

void AAAccumulateStage::render()
{
    int accumulatedSubpixels = painter().value<int>("accumulatedSubpixels");

    if (accumulatedSubpixels < 64)
    {
        accumulatedSubpixels++;
        m_program->setUniform("accumulatedSubpixels", accumulatedSubpixels);

        m_inputBuffer->bindTo(gl::GLenum::GL_TEXTURE0);
        m_program->setUniform("inputBuffer", 0);


        m_accumulateBuffer[m_targetBuffer? 1 : 0]->bindTo(gl::GLenum::GL_TEXTURE1);
        m_program->setUniform("accumulateBuffer", 1);

        bindFBO();

        m_accumulateBuffer[m_targetBuffer? 0 : 1]->attachTo(gl::GLenum::GL_COLOR_ATTACHMENT0);

        m_screenquad->draw(*m_program);
        releaseFBO();

        painter().setValue<int>("accumulatedSubpixels", accumulatedSubpixels);
        m_accumulateBuffer[m_targetBuffer? 1 : 0]->releaseFrom(gl::GLenum::GL_TEXTURE1);
        m_inputBuffer->releaseFrom(gl::GLenum::GL_TEXTURE0);

        m_targetBuffer = !m_targetBuffer;
        m_painter.setTexture(m_outputBufferName, m_accumulateBuffer[m_targetBuffer? 0 : 1]);
    }
}
