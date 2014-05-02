
#include "idpainter.h"

#include <glm/gtc/matrix_transform.hpp>

#include <QDebug>

#include <core/autotimer.h>
#include <core/mathmacros.h>
#include <core/glformat.h>
#include <core/camera.h>
#include <core/fileassociatedshader.h>
#include <core/framebufferobject.h>
#include <core/gpuquery.h>
#include <core/datacore/datablock.h>
#include <core/scenegraph/group.h>
#include <core/scenegraph/scenetraverser.h>
#include <core/scenegraph/drawvisitor.h>
#include <core/objloader.h>
#include <core/program.h>
#include <core/screenquad.h>
#include <core/navigation/arcballnavigation.h>
#include <core/navigation/flightnavigation.h>

#include <core/assimploader.h>


IdPainter::IdPainter(Camera * camera)
:   m_quad(nullptr)
,   m_program(nullptr)
,   m_fbo(nullptr)
,   m_flush(nullptr)
,   m_camera(camera)
{
}


IdPainter::~IdPainter()
{
    delete m_quad;

    delete m_program;
    delete m_fbo;
    delete m_flush;

    // AbstractScenePainter calls 'delete scene', therefore deleting the IdPainter
    // and constructing a new one (by loading a new file) causes a segmentation fault 
    m_scene = nullptr;
}

const bool IdPainter::initialize()
{
    AutoTimer t("Initialization of ID-Painter");

    m_quad = new ScreenQuad();


    // 
    m_program = new Program();
    m_program->attach(
        new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/colorId.frag"));
    m_program->attach(
        new FileAssociatedShader(GL_VERTEX_SHADER, "data/colorId.vert"));

    

    m_fbo = new FrameBufferObject(
        GL_RGBA8UI, GL_RGBA, GL_UNSIGNED_SHORT, GL_COLOR_ATTACHMENT0, true);

    return true;
}

void IdPainter::paint()
{
    AbstractPainter::paint();

    // t_samplerByName sampler;

    m_fbo->bind();
    SceneTraverser traverser;
    DrawVisitor drawVisitor( m_program, m_camera->transform() );
    traverser.traverse( *m_camera, drawVisitor );
    m_fbo->release();

    // sampler.clear();
    // sampler["source"] = m_fbo;

    // bindSampler(sampler, *m_flush);
    // m_quad->draw(*m_flush, nullptr);
    // releaseSampler(sampler);

}

void IdPainter::resize(  //probably never called anywhere?
    const int width
,   const int height)
{
    AbstractPainter::resize(width, height);

    m_camera->setViewport(width, height);

    m_fbo->resize(width, height);

    postShaderRelinked();

}


// void IdPainter::bindSampler(

//     const t_samplerByName & sampler
// ,   const Program & program)
// {
//     t_samplerByName::const_iterator i(sampler.cbegin());
//     const t_samplerByName::const_iterator iEnd(sampler.cend());

//     for(glm::uint slot(0); i != iEnd; ++i, ++slot)
//         i.value()->bindTexture2D(program, i.key(), slot);
// }

// void IdPainter::releaseSampler(
//     const t_samplerByName & sampler)
// {
//     t_samplerByName::const_iterator i(sampler.begin());
//     const t_samplerByName::const_iterator iEnd(sampler.cend());

//     for(; i != iEnd; ++i)
//         i.value()->releaseTexture2D();

// }

void IdPainter::setShading(char shader)
{
    
}

void IdPainter::setFrameBuffer(int frameBuffer)
{

}

void IdPainter::setEffect(int effect, bool value)
{

}

void IdPainter::postShaderRelinked()
{

}

Camera * IdPainter::camera()
{
    return m_camera;
}
