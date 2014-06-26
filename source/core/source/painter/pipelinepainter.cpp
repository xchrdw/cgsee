#include <core/painter/pipelinepainter.h>

#include <core/rendering/renderstage.h>
#include <core/rendering/pipelinebuilder.h>
#include <core/rendering/basicrenderstage.h>

#include <core/camera.h>
#include <core/scenegraph/group.h>
#include <core/screenquad.h>
#include <core/program.h>
#include <core/textureobject.h>
#include <core/fileassociatedshader.h>


PipelinePainter::PipelinePainter(Camera * camera)
	: PipelinePainter(camera, camera)
{

}

PipelinePainter::PipelinePainter(Camera * camera, Group * scene)
    : m_camera(camera)
    , m_scene(scene)
    , m_quad(new ScreenQuad())
    , m_flush(nullptr)
{

}

PipelinePainter::~PipelinePainter()
{
    clearRenderStages();
    delete m_flush;
    delete m_quad;
}

bool PipelinePainter::initialize()
{
    // Post Processing Shader
    m_flush = new Program();
    m_flush->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/dump.frag"));
    m_flush->attach(new FileAssociatedShader(GL_VERTEX_SHADER, "data/screenquad.vert"));

    addRenderStage(new BasicRenderStage(*this));

    return true;
}


void PipelinePainter::pipelineConfigChanged()
{
    //TODO think!
}
void PipelinePainter::sceneChanged()
{
    //TODO think!
}

void PipelinePainter::paint()
{
    AbstractPainter::paint();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    foreach (RenderStage * renderStage, m_stages)
    {
        renderStage->render();
    }

    TextureObject * texture = getTexture("normalz");
    if(!texture)
        return;

    texture->bindTo(GL_TEXTURE0);
    m_flush->setUniform("selectedBuffer", 0);
    m_quad->draw(*m_flush);
    texture->releaseFrom(GL_TEXTURE0);
}

void PipelinePainter::resize(const int width, const int height)
{
    AbstractPainter::resize(width, height);
    foreach (RenderStage * renderStage, m_stages)
    {
        renderStage->resize(width, height);
    }
    camera()->setViewport(width, height);
}

void PipelinePainter::reloadShaders()
{
    foreach (RenderStage * renderStage, m_stages)
    {
        renderStage->reloadShaders();
    }
}

// TODO remove as soon as possible
void PipelinePainter::selectCamera(const QString cameraName)
{

}

void PipelinePainter::selectRendering(const QString rendering)
{

}

void PipelinePainter::setShading(char shader)
{

}

void PipelinePainter::setFrameBuffer(int frameBuffer)
{

}

void PipelinePainter::setEffect( int effect, bool active)
{

}

void PipelinePainter::setStereoCameraSeparation(AbstractProperty & property)
{

}

void PipelinePainter::setConvergentCameraFocus(AbstractProperty & property)
{

}

void PipelinePainter::postShaderRelinked()
{

}

RenderingPass * PipelinePainter::getSharedPass()
{
    return nullptr;
}
// TODO end

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

TextureObject * PipelinePainter::getTexture(QString name)
{
    return m_textures.value(name, nullptr);
}

bool PipelinePainter::textureExists(QString name)
{
    return m_textures.contains(name);
}

void PipelinePainter::setTexture(QString name, TextureObject * texture)
{
    m_textures[name] = texture;
}

bool PipelinePainter::addTexture(QString name, TextureObject * texture)
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
