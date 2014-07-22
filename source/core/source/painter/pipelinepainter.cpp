#include <core/painter/pipelinepainter.h>

#include <core/rendering/abstractrenderstage.h>
#include <core/rendering/pipelinebuilder.h>
#include <core/rendering/renderstage.h>

#include <core/camera/abstractcamera.h>
#include <core/scenegraph/group.h>
#include <core/screenquad.h>
#include <core/program.h>
#include <core/textureobject.h>
#include <core/fileassociatedshader.h>


PipelinePainter::PipelinePainter(AbstractCamera * camera)
	: PipelinePainter(camera, camera)
{

}

PipelinePainter::PipelinePainter(AbstractCamera * camera, Group * scene)
    : m_camera(camera)
    , m_scene(scene)
    , m_quad(new ScreenQuad())
    , m_flush(nullptr)
    //, m_coordFBO(new glow::FrameBufferObject())
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

    addRenderStage(new RenderStage(*this));

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

    foreach (AbstractRenderStage * renderStage, m_stages)
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
    foreach (AbstractRenderStage * renderStage, m_stages)
    {
        renderStage->resize(width, height);
    }
    camera()->setViewport(width, height);
}

void PipelinePainter::reloadShaders()
{
    foreach (AbstractRenderStage * renderStage, m_stages)
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
void PipelinePainter::setViewFrustumCulling(bool viewFrustumCullingEnabled)
{

}

bool PipelinePainter::isViewFrustumCullingEnabled()
{

}

// TODO end

//from coordinateProvider
unsigned int PipelinePainter::getObjectID(unsigned int x, unsigned int y)
{
//  TODO glow integration necessary to use FBO
//    TextureObject texture = getTexture("colorID");//TODO ---> constant?

//    if (!texture || x >= texture->width() || y >= texture->height())
//        return 0;

//    m_coordFBO->bind();
//    m_coordFBO->attachTexture2D(GL_COLOR_ATTACHMENT0, texture);
//    glReadBuffer(GL_COLOR_ATTACHMENT0);
//    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

//    float data[4];
//    glReadPixels(x, fbo->height()-y, 1, 1, GL_RGBA, GL_FLOAT, data);
//    fbo->release();

//    m_pass->setActive(status);

//    return 255*data[0] + 255*data[1]*255 + 255*data[2]*255*255 + 255*data[3]*255*255*255;
    return 0;
}

glm::dvec3 PipelinePainter::get3DPoint(unsigned int x, unsigned int y)
{
//  TODO glow integration necessary to use FBO
//    if (!m_pass) return glm::dvec3(0.0f,0.0f,0.0f);

//    bool status = m_pass->isActive();

//    m_pass->setActive(true);
//    m_pass->applyIfActive();

//    FrameBufferObject * fbo = m_pass->output();

//    if (x >= fbo->width() || y >= fbo->height())
//        return glm::dvec3(0.0f,0.0f,0.0f);

//    fbo->bind();
//    glFlush();
//    glFinish();
//    // glReadBuffer(GL_COLOR_ATTACHMENT0);
//    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

//    float z;
//    y = fbo->height()-y;
//    glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z);
//    fbo->release();

//    m_pass->setActive(status);

//    return unproject((float) x, (float) y, z);
    return glm::dvec3();
}

glm::ivec2 PipelinePainter::get2DPoint(unsigned int x, unsigned int y)
{
//  TODO glow integration necessary to use FBO
//    if (!m_pass) return glm::dvec3(0.0f,0.0f,0.0f);

//    bool status = m_pass->isActive();

//    m_pass->setActive(true);
//    m_pass->applyIfActive();

//    FrameBufferObject * fbo = m_pass->output();

//    if (x >= fbo->width() || y >= fbo->height())
//        return glm::dvec3(0.0f,0.0f,0.0f);

//    fbo->bind();
//    glFlush();
//    glFinish();
//    // glReadBuffer(GL_COLOR_ATTACHMENT0);
//    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

//    float z;
//    y = fbo->height()-y;
//    glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z);
//    fbo->release();

//    m_pass->setActive(status);

//    return unproject((float) x, (float) y, z);
    return glm::ivec2(x, y);
}

glm::dvec3 PipelinePainter::unproject(float x, float y, float z)
{
    GLdouble modelview[16];
    GLdouble projection[16];
    GLint viewport[4];

    glm::mat4 viewMat = camera()->view();
    glm::mat4 projectionMat = camera()->projection();

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            modelview[i*4+j] = viewMat[i][j];
            projection[i*4+j] = projectionMat[i][j];
        }
    }

    viewport[0] = viewport[1] = 0;
    viewport[2] = camera()->viewport().x;
    viewport[3] = camera()->viewport().y;

    glm::dvec3 position;
    gluUnProject( x, y, z, modelview, projection, viewport, &position.x, &position.y, &position.z );

    return position;
}

Group * PipelinePainter::scene()
{
    return m_scene;
}

const ScreenQuad * PipelinePainter::screenQuad()
{
    return m_quad;
}

AbstractCamera * PipelinePainter::camera()
{
    return m_camera;
}

void PipelinePainter::addRenderStage(AbstractRenderStage * renderStage)
{
    m_stages.push_back(renderStage);
}

void PipelinePainter::setScene(Group * scene)
{
    m_scene = scene;
    sceneChanged();
}

void PipelinePainter::setCamera(AbstractCamera * camera)
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
    for(QList<AbstractRenderStage*>::iterator i = m_stages.begin(); i != m_stages.end(); i++)
    {
        delete (*i);
    }
    m_stages.clear();
}
