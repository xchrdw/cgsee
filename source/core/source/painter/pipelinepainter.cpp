#include <core/painter/pipelinepainter.h>

#include <core/camera.h>
#include <core/scenegraph/group.h>
#include <core/screenquad.h>
#include <core/program.h>

PipelinePainter(Camera * camera, Group * scene)
    : m_camera(camera)
    , m_scene(scene)
    , m_quad(new ScreenQuad())
    , m_stages(QList<RenderStage *>())
    , m_textures(QMap<QString, GLuint>())
{

}

virtual ~PipelinePainter()
{
    clearRenderStages();
}

virtual void pipelineConfigChanged()
{
    // select and configure appropriate builder
    setupPipeline(PipelineBuilder());
}

void sceneChanged()
{
    foreach (RenderStage * renderStage, m_stages)
    {
        renderStage->sceneChanged();
    }
}

void viewChanged()
{
    foreach (RenderStage * renderStage, m_stages)
    {
        renderStage->viewChanged();
    }
}

void paint()
{
    foreach (RenderStage * renderStage, m_stages)
    {
        renderStage->render();
    }
    // show ScreenQuad
}

void resize(const int width, const int height)
{
    foreach (RenderStage * renderStage, m_stages)
    {
        renderStage->resize(width, height);
    }
}

void reloadShaders()
{
    foreach (RenderStage * renderStage, m_stages)
    {
        renderStage->reloadShaders();
    }
}


const Group * scene()
{
    return m_scene;
}

const ScreenQuad * screenQuad()
{
    return m_quad;
}

Camera * camera()
{
    return m_camera;
}

void addRenderStage(RenderStage * renderStage)
{
    m_stages.push_back(renderStage);
}

void setScene(Group * scene)
{
    m_scene = scene;
    sceneChanged();
}

void setCamera(Camera * camera)
{
    m_camera = camera;
    pipelineConfigChanged();
}

GLuint getTexture(QString name)
{
    return m_texture.value(name, 0);
}

bool textureExists(QString name)
{
    return m_texture.contains(name);
}

void setTexture(QString name, GLuint texture)
{
    m_texture[name] = texture;
}

bool addTexture(QString name, GLuint texture)
{
    if(textureExists(name))
        return false;
    setTexture(name, texture);
    return true;
}

void removeTexture(QString name)
{
    m_textures.remove(name);
}

void setupPipeline(PipelineBuilder & builder)
{
    clearRenderStages();
    //TODO invoke builder
}

void clearRenderStages()
{
    for(QList::iterator i = m_stages.begin(); i != m_stages.end(); i++)
    {
        delete (*i.pointer);
    }
    m_stages.clear();
}


#include <core/painter/pipelinepainter.h>

PipelinePainter::PipelinePainter(Camera * camera)
	: AbstractScenePainter()
	, m_camera(camera)
{

}

PipelinePainter::~PipelinePainter()
{

}
