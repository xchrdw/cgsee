#include <core/painter/pipelinepainter.h>

#include <core/rendering/renderstage.h>
#include <core/rendering/pipelinebuilder.h>

#include <core/camera.h>
#include <core/scenegraph/group.h>
#include <core/screenquad.h>
#include <core/program.h>

PipelinePainter::PipelinePainter(Camera * camera, Group * scene)
    : m_camera(camera)
    , m_scene(scene)
    , m_quad(new ScreenQuad())
    , m_stages(QList<RenderStage *>())
    , m_textures(QMap<QString, GLuint>())
{

}

PipelinePainter::~PipelinePainter()
{
    clearRenderStages();
}

void PipelinePainter::pipelineConfigChanged()
{
    // select and configure appropriate builder
    // setupPipeline(PipelineBuilder(*this));
}

void PipelinePainter::sceneChanged()
{
    //TODO think!
}

void PipelinePainter::viewChanged()
{
    //TODO think!
}

void PipelinePainter::paint()
{
    foreach (RenderStage * renderStage, m_stages)
    {
        renderStage->render();
    }
    // show ScreenQuad
}

void PipelinePainter::resize(const int width, const int height)
{
    foreach (RenderStage * renderStage, m_stages)
    {
        renderStage->resize(width, height);
    }
}

void PipelinePainter::reloadShaders()
{
    foreach (RenderStage * renderStage, m_stages)
    {
        renderStage->reloadShaders();
    }
}


Group * PipelinePainter::scene()
{
    return m_scene;
}

const ScreenQuad * PipelinePainter::screenQuad()
{
    return m_quad;
}

Camera * PipelinePainter::camera()
{
    return m_camera;
}

void PipelinePainter::addRenderStage(RenderStage * renderStage)
{
    m_stages.push_back(renderStage);
}

void PipelinePainter::setScene(Group * scene)
{
    m_scene = scene;
    sceneChanged();
}

void PipelinePainter::setCamera(Camera * camera)
{
    m_camera = camera;
    pipelineConfigChanged();
}

bool PipelinePainter::isSceneInvalid()
{
    return true;//TODO
}

bool PipelinePainter::isViewInvalid()
{
    return true;//TODO
}

GLuint PipelinePainter::getTexture(QString name)
{
    return m_textures.value(name, 0);
}

bool PipelinePainter::textureExists(QString name)
{
    return m_textures.contains(name);
}

void PipelinePainter::setTexture(QString name, GLuint texture)
{
    m_textures[name] = texture;
}

bool PipelinePainter::addTexture(QString name, GLuint texture)
{
    if(textureExists(name))
        return false;
    setTexture(name, texture);
    return true;
}

void PipelinePainter::removeTexture(QString name)
{
    m_textures.remove(name);
}

void PipelinePainter::setupPipeline(PipelineBuilder & builder)
{
    clearRenderStages();
    //TODO invoke builder
}

void PipelinePainter::clearRenderStages()
{
    for(QList<RenderStage*>::iterator i = m_stages.begin(); i != m_stages.end(); i++)
    {
        delete (*i);
    }
    m_stages.clear();
}
