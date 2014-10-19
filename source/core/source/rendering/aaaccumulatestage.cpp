
#include <core/rendering/aaaccumulatestage.h>

#include <glbinding/gl/bitfield.h>
#include <glbinding/gl/enum.h>

#include <globjects/Framebuffer.h>
#include <globjects/Program.h>
#include <globjects/Texture.h>
#include <globjects/Shader.h>
#include <globjects/base/File.h>

#include <core/camera/abstractcamera.h>
#include <core/painter/abstractpainter.h>
#include <core/painter/pipelinepainter.h>
#include <core/screenquad.h>

AAAccumulateStage::AAAccumulateStage(PipelinePainter & painter, const QString & inputBufferName, const QString & outputBufferName)
    : AbstractPostProcessingStage(painter)
    , m_outputBufferName(outputBufferName)
    , m_targetBuffer(false)
    , m_inputBuffer(painter.getTexture(inputBufferName))
{

	m_accumulateBuffer[0] = globjects::Texture::createDefault(gl::GL_TEXTURE_2D);
	m_accumulateBuffer[1] = globjects::Texture::createDefault(gl::GL_TEXTURE_2D);

    m_painter.addTexture(m_outputBufferName, m_accumulateBuffer[m_targetBuffer? 0 : 1]);

	m_program->attach(new globjects::Shader(gl::GL_FRAGMENT_SHADER, new globjects::File("data/aaaccumulate.frag")));
	m_program->attach(new globjects::Shader(gl::GL_VERTEX_SHADER, new globjects::File("data/screenquad.vert")));

	resize(painter.camera()->viewport().x, painter.camera()->viewport().y);
}

AAAccumulateStage::~AAAccumulateStage(void)
{

}

void AAAccumulateStage::reloadShaders()
{

}

void AAAccumulateStage::resize(const int width, const int height)
{
	m_accumulateBuffer[0]->image2D(0, gl::GL_RGBA32F, width, height, 0, gl::GL_RGBA, gl::GL_FLOAT, nullptr);
	m_accumulateBuffer[1]->image2D(0, gl::GL_RGBA32F, width, height, 0, gl::GL_RGBA, gl::GL_FLOAT, nullptr);
}

void AAAccumulateStage::render()
{
    int accumulatedSubpixels = painter().value<int>("accumulatedSubpixels");

    if (accumulatedSubpixels < 64)
    {
        accumulatedSubpixels++;
        m_program->setUniform("accumulatedSubpixels", accumulatedSubpixels);

		m_inputBuffer->bindActive(gl::GL_TEXTURE0);
        m_program->setUniform("inputBuffer", 0);


		m_accumulateBuffer[m_targetBuffer ? 1 : 0]->bindActive(gl::GL_TEXTURE1);
        m_program->setUniform("accumulateBuffer", 1);

		m_fbo->attachTexture(gl::GL_COLOR_ATTACHMENT0, m_accumulateBuffer[m_targetBuffer ? 0 : 1]);
        m_screenquad->draw(*m_program);
        releaseFBO();

        painter().setValue<int>("accumulatedSubpixels", accumulatedSubpixels);
		m_accumulateBuffer[m_targetBuffer ? 1 : 0]->unbindActive(gl::GL_TEXTURE1);
		m_inputBuffer->unbindActive(gl::GL_TEXTURE0);

        m_targetBuffer = !m_targetBuffer;
        m_painter.setTexture(m_outputBufferName, m_accumulateBuffer[m_targetBuffer? 0 : 1]);
    }
}
