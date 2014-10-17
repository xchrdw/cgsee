#include <core/painter/pipelinepainter.h>

#include <string>
#include <glbinding/gl/types.h>
#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>
#include <glbinding/gl/bitfield.h>

#include <globjects/Program.h>
#include <globjects/Texture.h>
#include <globjects/base/File.h>

#include <core/rendering/abstractrenderstage.h>
#include <core/rendering/renderstage.h>
#include <core/camera/abstractcamera.h>
#include <core/scenegraph/group.h>
#include <core/screenquad.h>
#include <core/rendering/rasterizationpipelinebuilder.h>

#include <core/gpuquery.h>


const std::string PipelinePainter::VIEWPORT_UNIFORM("viewport");
const std::string PipelinePainter::VIEW_UNIFORM("view");
const std::string PipelinePainter::PROJECTION_UNIFORM("projection");
const std::string PipelinePainter::TRANSFORM_UNIFORM("transform");
const std::string PipelinePainter::TRANSFORMINVERSE_UNIFORM("transformInverse");
const std::string PipelinePainter::ZNEAR_UNIFORM("znear");
const std::string PipelinePainter::ZFAR_UNIFORM("zfar");
const std::string PipelinePainter::CAMERAPOSITION_UNIFORM("cameraposition");

PipelinePainter::PipelinePainter(AbstractCamera * camera)
	: PipelinePainter(camera, camera)
{

}

PipelinePainter::PipelinePainter(AbstractCamera * camera, Group * scene)
    : PropertyGroup()
    , m_quad(new ScreenQuad())
    , m_flush(nullptr)
    //, m_coordFBO(new globjects::Framebuffer())
{
    addProperty<bool>("viewInvalid", true);
    addProperty<std::string>("selectedBufferName", "");
    assignScene(scene);
    assignCamera(camera);
}

PipelinePainter::~PipelinePainter()
{
    clearRenderStages();
    delete m_quad;
}

bool PipelinePainter::initialize()
{
    // Post Processing Shader
    m_flush = new globjects::Program();
    
    m_flush->attach(new globjects::Shader(gl::GL_FRAGMENT_SHADER, new globjects::File("data/dump.frag")));
    m_flush->attach(new globjects::Shader(gl::GL_VERTEX_SHADER, new globjects::File("data/screenquad.vert")));

    //TODO should be chosen according to property
    AbstractPipelineBuilder * builder = new RasterizationPipelineBuilder(*this);
    builder->build();
    delete builder;

    return true;
}


void PipelinePainter::pipelineConfigChanged()
{
    //TODO think!
}

void PipelinePainter::sceneChanged()
{
    AbstractScenePainter::sceneChanged();
    //TODO think!
}

void PipelinePainter::cameraChanged()
{
    AbstractScenePainter::cameraChanged();
    pipelineConfigChanged();
}

void PipelinePainter::onViewChanged()
{
    setValue<bool>("viewInvalid", true);
}

void PipelinePainter::paint()
{
    gl::glBindFramebuffer(gl::GL_FRAMEBUFFER, 0);
    AbstractPainter::paint();
    gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);
    glError();

    setView(camera()->view());
    setProjection(camera()->projection());

    foreach (AbstractRenderStage * renderStage, m_stages)
    {
        renderStage->render();
    }

    globjects::Texture * texture = getTexture(QString::fromStdString(value<std::string>("selectedBufferName")));
    if(!texture)
        return;

    texture->bindActive(gl::GL_TEXTURE0);
    m_flush->setUniform("selectedBuffer", 0);
    m_quad->draw(*m_flush);

    setValue<bool>("viewInvalid", false);
    texture->unbindActive(gl::GL_TEXTURE0);
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
    return false;
}

// TODO end

//from coordinateProvider
unsigned int PipelinePainter::getObjectID(unsigned int x, unsigned int y)
{
//  TODO globjects integration necessary to use FBO
//    globjects::Texture texture = getTexture("colorID");//TODO ---> constant?

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
//  TODO globjects integration necessary to use FBO
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
//  TODO globjects integration necessary to use FBO
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

void PipelinePainter::selectionChanged(QMap<unsigned int, Node *> selectedNodes)
{
    //TODO implement selection visualisation
//    m_selectionBBox->invalidate();
//    for ( auto node : m_selectedNodes )
//    {
//        m_selectionBBox->extend(node->boundingBox());
//    }
//    m_selectionBBox->llf();
//    m_selectionBBox->urb();
}

glm::vec3 PipelinePainter::unproject(float x, float y, float z)
{
    glm::mat4 modelview = camera()->view();
    glm::mat4 projection = camera()->projection();
    glm::vec4 viewport = glm::vec4(0.f, 0.f, static_cast<float>(camera()->viewport().x), static_cast<float>(camera()->viewport().y));

    glm::dvec3 position;

    return glm::unProject(glm::vec3(x, y, z), modelview, projection, viewport);
}

const ScreenQuad * PipelinePainter::screenQuad()
{
    return m_quad;
}

void PipelinePainter::addRenderStage(AbstractRenderStage * renderStage)
{
    m_stages.push_back(renderStage);
}

bool PipelinePainter::isSceneInvalid()
{
    return true;//TODO
}

bool PipelinePainter::isViewInvalid()
{
    return true;//TODO
}

globjects::Texture* PipelinePainter::getTexture(QString name)
{
    return m_textures.value(name, nullptr);
}

bool PipelinePainter::textureExists(QString name)
{
    return m_textures.contains(name);
}

void PipelinePainter::setTexture(QString name, globjects::Texture * texture)
{
    m_textures[name] = texture;
}

bool PipelinePainter::addTexture(QString name, globjects::Texture * texture)
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

const glm::mat4 &  PipelinePainter::view() const
{
    return m_view;
}

const glm::mat4 & PipelinePainter::projection() const
{
    return m_projection;
}

const glm::mat4 & PipelinePainter::projectionInverse() const
{
    return m_projectionInverse;
}

const glm::mat4 & PipelinePainter::transform() const
{
    return m_transform;
}

const glm::mat4 & PipelinePainter::transformInverse() const
{
    return m_transformInverse;
}

void PipelinePainter::setView(const glm::mat4 & view)
{
    m_view = view;
    recalculateTransform();
}

void PipelinePainter::setProjection(const glm::mat4 & projection)
{
    m_projection = projection;
    m_projectionInverse = glm::inverse(projection);
    recalculateTransform();
}

void PipelinePainter::setCameraUniforms(globjects::Program & program)
{
    program.setUniform(PROJECTION_UNIFORM, m_projection);
    program.setUniform(VIEWPORT_UNIFORM, m_viewport);
    program.setUniform(ZNEAR_UNIFORM, m_zNear);
    program.setUniform(ZFAR_UNIFORM, m_zFar);

    program.setUniform(VIEW_UNIFORM, m_view);
    program.setUniform(TRANSFORM_UNIFORM, m_transform);
    program.setUniform(TRANSFORMINVERSE_UNIFORM, m_transformInverse);
    program.setUniform(CAMERAPOSITION_UNIFORM, m_eye);
}

void PipelinePainter::recalculateTransform()
{
    m_transform = m_projection * m_view;
    m_transformInverse = glm::inverse(m_transform);
}

void PipelinePainter::clearPipeline()
{
    clearRenderStages();
    // removes properties
    //clear();
}

void PipelinePainter::clearRenderStages()
{
    for(QList<AbstractRenderStage*>::iterator i = m_stages.begin(); i != m_stages.end(); i++)
    {
        delete (*i);
    }
    m_stages.clear();
}
