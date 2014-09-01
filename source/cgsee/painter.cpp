#include <glm/gtc/matrix_transform.hpp>
#include <QDebug>
#include <QWidget>

#include "painter.h"

#include <core/autotimer.h>
#include <core/mathmacros.h>
#include <core/glformat.h>
#include <core/camera.h>
#include <core/parallelcamera.h>
#include <core/convergentcamera.h>
#include <core/fileassociatedshader.h>
#include <core/framebufferobject.h>
#include <core/gpuquery.h>
#include <core/datacore/datablock.h>
#include <core/scenegraph/group.h>
#include <core/scenegraph/scenetraverser.h>
#include <core/scenegraph/drawvisitor.h>
#include <core/scenegraph/cullingvisitor.h>
#include <core/program.h>
#include <core/screenquad.h>
#include <core/rendering/ssaoeffect.h>
#include <core/rendering/blureffect.h>
#include <core/rendering/shadowmapping.h>
#include <core/rendering/normalzpass.h>
#include <core/rendering/coloridpass.h>
#include <core/rendering/boundingboxpass.h>
#include <core/rendering/lightsource.h>
#include <core/rendering/lightingsystem.h>

#include <core/property/advancedlistproperty.h>
#include <core/property/valueproperty.h>
#include <core/property/limitedproperty.h>
#include <core/property/propertylist.h>
#include <core/property/announcer.h>
#include <gui/propertywidgetbuilder.h>



//for phong, flat and gouraud
static const QString LIGHTDIR_UNIFORM ("lightdir");
static const QString LIGHTDIR_UNIFORM2 ("lightdir2");
static const QString LIGHTAMBIENTGLOBAL_UNIFORM ("lightambientglobal");
static const QString LIGHT_UNIFORM ("light");
static const QString LIGHT_UNIFORM2 ("light2");
static const QString MATERIAL_UNIFORM ("material");
static const QString LIGHTPOSITION_UNIFORM ("lightposition");
//gooch
static const QString WARMCOLDCOLOR_UNIFORM ("warmcoldcolor");


Painter::Painter(Camera * camera)
:   AbstractScenePainter()
,   m_quad(nullptr)
,   m_normals(nullptr)
,   m_normalz(nullptr)
,   m_wireframe(nullptr)
,   m_primitiveWireframe(nullptr)
,   m_solidWireframe(nullptr)
,   m_flat(nullptr)
,   m_gouraud(nullptr)
,   m_phong(nullptr)
,   m_gooch(nullptr)
,   m_colorId(nullptr)
,   m_boundingBox(nullptr)
,   m_useProgram(nullptr)
,   m_fboColor(nullptr)
,   m_fboTemp(nullptr)
,   m_fboActiveBuffer(nullptr)
,   m_flush(nullptr)
,   m_shadows(nullptr)
,   m_camera(camera)
,   m_useColor(true)
,   m_passes()
,   m_lightingSystem(nullptr)
{

}

Painter::~Painter()
{
    delete m_quad;
    delete m_normals;
    delete m_normalz;
    delete m_shadows;
    delete m_shadowBlur;
    delete m_ssao;
    delete m_ssaoBlur;
    delete m_colorId;
    delete m_boundingBox;
    delete m_flat;
    delete m_gouraud;
    delete m_phong;
    delete m_gooch;
    delete m_wireframe;
    delete m_primitiveWireframe;
    delete m_solidWireframe;
    delete m_fboColor;
    delete m_lightingSystem;

    delete m_flush;
}

void Painter::setConvergentCameraFocus(AbstractProperty & p)
{
    if(dynamic_cast<ConvergentCamera*>(m_camera))
    {
        qDebug("set ConvergentCameraFocus");
        //((ConvergentCamera*)m_camera)->setFocusDistance(p.to<LimitedProperty<float>>()->value());
        ConvergentCamera * convergent = dynamic_cast<ConvergentCamera*>(m_camera->activeImplementation());
        if (convergent == nullptr) {
            qDebug() << "Trying to configure ConvergentCamera but " << m_camera->selectedImplementation() << " is active.";
            exit(2);
        }
        convergent->setFocusDistance(p.to<LimitedProperty<float>>()->value());
        //resize(m_camera->viewport().x,m_camera->viewport().y);
    }
    
}

void Painter::setStereoCameraSeparation(AbstractProperty & p)
{
    
    if(dynamic_cast<AbstractStereoCamera*>(m_camera))
    {
        qDebug("set StereoCameraSeparation");
       ((AbstractStereoCamera*)m_camera)->setCameraSeparation(p.to<LimitedProperty<float>>()->value());
       //resize(m_camera->viewport().x,m_camera->viewport().y);
    }
  
}

const bool Painter::initialize()
{
    AutoTimer t("Initialization of Painter");

    if(m_scene) {
        m_camera->append(m_scene);
    }

    m_camera->setPainter(this);

    LimitedProperty<float> * cameraSeparationLevel = new LimitedProperty<float>("cameraSeparation", "Camera-Separation:", 0.5f, 0.0f, 12.3f);
    LimitedProperty<float> * focusdistanceLevel = new LimitedProperty<float>("focusdistance", "Focus-Distance:", 0.5f, 0.0f, 12.3f);
    
    m_propertylist->add(cameraSeparationLevel);
    cameraSeparationLevel->subscribe(LimitedProperty<float>::kValueChanged, this, &Painter::setStereoCameraSeparation);

    m_quad = new ScreenQuad();

    FileAssociatedShader * depth_util = new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/depth_util.frag");

    // NORMALS
    m_normals = new Program();
    m_normals->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/normals.frag"));
    m_normals->attach(new FileAssociatedShader(GL_GEOMETRY_SHADER, "data/normals.geo"));
    m_normals->attach(new FileAssociatedShader(GL_VERTEX_SHADER, "data/normals.vert"));


    FileAssociatedShader * screenQuadShader = new FileAssociatedShader(GL_VERTEX_SHADER, "data/screenquad.vert");

    FileAssociatedShader *wireframeShader = new FileAssociatedShader(GL_VERTEX_SHADER, "data/wireframe/wireframe.vert");
    FileAssociatedShader *wireframeShaderGEO = new FileAssociatedShader(GL_GEOMETRY_SHADER, "data/wireframe/wireframe.geo");

    //Wireframe
    m_wireframe = new Program();
    m_wireframe->attach(wireframeShaderGEO);
    m_wireframe->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/wireframe/wireframe.frag"));
    m_wireframe->attach(wireframeShader);

    //Primitive Wireframe
    m_primitiveWireframe = new Program();
    m_primitiveWireframe->attach(
        new FileAssociatedShader(GL_GEOMETRY_SHADER, "data/wireframe/primitiveWireframe.geo"));
    m_primitiveWireframe->attach(
        new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/wireframe/primitiveWireframe.frag"));
    m_primitiveWireframe->attach(
        new FileAssociatedShader(GL_VERTEX_SHADER, "data/wireframe/primitiveWireframe.vert"));

    //Solid Wireframe
    m_solidWireframe = new Program();
    m_solidWireframe->attach(wireframeShaderGEO);
    m_solidWireframe->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/wireframe/wireframeSolid.frag"));
    m_solidWireframe->attach(wireframeShader);

    FileAssociatedShader *phongLighting = new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/shading/phongLighting.frag");

    //FLAT
    m_flat = new Program();
    m_flat->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/shading/flat.frag"));
    m_flat->attach(phongLighting);
    m_flat->attach(new FileAssociatedShader(GL_VERTEX_SHADER, "data/shading/flat.vert"));
    m_flat->attach(new FileAssociatedShader(GL_GEOMETRY_SHADER, "data/shading/flat.geo"));

    //GOURAUD
    m_gouraud = new Program();
    m_gouraud->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/shading/gouraud.frag"));
    m_gouraud->attach(new FileAssociatedShader(GL_VERTEX_SHADER, "data/shading/gouraud.vert"));
    m_gouraud->attach(new FileAssociatedShader(GL_VERTEX_SHADER, "data/shading/phongLighting.vert"));

    //PHONG
    m_phong = new Program();
    m_phong->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/shading/phong.frag"));
    //m_phong->attach(phongLighting);
    m_phong->attach(new FileAssociatedShader(GL_VERTEX_SHADER, "data/shading/phong.vert"));

    //GOOCH
    m_gooch = new Program();
    m_gooch->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/shading/gooch.frag"));
    m_gooch->attach(new FileAssociatedShader(GL_VERTEX_SHADER, "data/shading/gooch.vert"));

    //set UNIFORMS for selected shader
    m_useProgram = m_phong;
    //setUniforms();

    // Post Processing Shader
    m_flush = new Program();
    m_flush->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/flush.frag"));
    m_flush->attach(screenQuadShader);

    m_fboColor = new FrameBufferObject(GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0, true);
    m_fboTemp = new FrameBufferObject(GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0, true);

    m_normalz = new NormalzPass(m_camera, depth_util);
    m_lightsource = new LightSourcePass(m_camera, depth_util);
    m_shadows = new ShadowMappingPass(m_camera, depth_util, m_lightsource);
    m_shadowBlur = new BlurEffect(m_camera, m_quad, screenQuadShader, m_shadows, m_fboTemp);
    m_ssao = new SSAOEffect(m_camera, m_quad, screenQuadShader, m_normalz->output());
    m_ssaoBlur = new BlurEffect(m_camera, m_quad, screenQuadShader, m_ssao, m_fboTemp);
    m_colorId = new ColorIdPass(m_camera);
    m_boundingBox = new BoundingBoxPass(m_camera, m_fboColor);
    
    m_passes.append(m_normalz);
    m_passes.append(m_lightsource);
    m_passes.append(m_shadows);
    m_passes.append(m_shadowBlur);
    m_passes.append(m_ssao);
    m_passes.append(m_ssaoBlur);
    m_passes.append(m_colorId);
    m_passes.append(m_boundingBox);


    m_fboActiveBuffer = m_fboColor;

    PropertyWidgetBuilder builder;
    builder.buildWidget(m_propertylist->list());

    builder.retainWidget()->hide();

    m_propertylist->add(focusdistanceLevel);
    focusdistanceLevel->subscribe(LimitedProperty<float>::kValueChanged, this, &Painter::setConvergentCameraFocus);

    PropertyWidgetBuilder builder2;
    builder2.buildWidget(m_propertylist->list());
    builder2.retainWidget()->hide();
    //builder2.retainWidget()->setFocusPolicy(Qt::NoFocus);

    // Lighting system code starts here
    m_lightingSystem = new LightingSystem();
    m_lightingSystem->initBuffers();
  
    m_lightingSystem->addDirectionalLight(glm::normalize(glm::vec3(1.f, 1.f, 1.f)), glm::vec3(1.f, 1.f, 1.f));
    m_lightingSystem->addDirectionalLight(glm::normalize(glm::vec3(-1.f, -1.f, -1.f)), glm::vec3(1.f, 1.f, 0.9f));
    m_lightingSystem->addPointLight(glm::vec4(1.f, 4.f, 3.f, 1.f), glm::vec3(1.f, 0.f, 0.f), 6.f);
    m_lightingSystem->addSpotLight(glm::vec4(0.f, 10.f, 0.f, 1.f), glm::vec3(0.f, -1.f, 0.f), glm::vec3(0.f, 0.f, 1.f), 5.f, 4.f);

    return true;
}

void Painter::setUniforms()
{
    if(m_useProgram == m_flat || m_useProgram == m_gouraud)
    {
        m_useProgram->setUniform(LIGHTDIR_UNIFORM, glm::vec3(0.0,6.5,7.5));
        m_useProgram->setUniform(LIGHTDIR_UNIFORM2, glm::vec3(0.0,-8.0,7.5));

        m_useProgram->setUniform(LIGHTAMBIENTGLOBAL_UNIFORM, glm::vec4(0.0));

        glm::mat4 lightMat;
        lightMat[0] = glm::vec4(0.0,0.0,0.0,1.0);        //ambient
        lightMat[1] = glm::vec4(0.2,0.2,0.2,1.0);        //diffuse
        lightMat[2] = glm::vec4(0.0,0.0,0.8,1.0);        //specular
        lightMat[3] = glm::vec4(0.002,0.002,0.0004,1.4); //attenuation1, attenuation2, attenuation3, shininess
        m_useProgram->setUniform(LIGHT_UNIFORM, lightMat, false);

        glm::mat4 lightMat2;
        lightMat2[0] = glm::vec4(0.0,0.0,0.0,1.0);        //ambient
        lightMat2[1] = glm::vec4(0.1,0.1,0.1,1.0);        //diffuse
        lightMat2[2] = glm::vec4(0.8,0.0,0.0,1.0);        //specular
        lightMat2[3] = glm::vec4(0.002,0.002,0.0004,1.4); //attenuation1, attenuation2, attenuation3, shininess

        m_useProgram->setUniform(LIGHT_UNIFORM2, lightMat2, false);

//         glm::mat4 materialCoeff;
//         materialCoeff[0] = glm::vec4(0.1,0.1,0.1,1.0);    //ambient
//         materialCoeff[1] = glm::vec4(1.0,1.0,1.0,1.0);    //diffuse
//         materialCoeff[2] = glm::vec4(1.0,1.0,1.0,1.0);    //specular
//         materialCoeff[3] = glm::vec4(0,0,0,0);            //emission
// 
//         m_useProgram->setUniform(MATERIAL_UNIFORM, materialCoeff);
    }

    else if(m_useProgram == m_gooch)
    {
        m_useProgram->setUniform(LIGHTPOSITION_UNIFORM, glm::vec3(-2.0,0.0,2.0));

        glm::mat4 warmColdColor;

        warmColdColor[0] = glm::vec4(0.75, 0.75, 0.75, 0.0); //surface color
        warmColdColor[1] = glm::vec4(0.6, 0.6, 0.0, 0.0);    //warm color
        warmColdColor[2] = glm::vec4(0.0, 0.0, 0.6, 0.0);    //cold color
        warmColdColor[3] = glm::vec4(0.45,0.45,0,0);         //Diffuse Warm, DiffuseCool
        m_useProgram->setUniform(WARMCOLDCOLOR_UNIFORM, warmColdColor);
    }

    else if (m_useProgram == m_phong)
    {
            // Since setUniforms is called from postShaderRelink
			// Bugfix: the program must be bound to query binding locations for the uniform blocks in the program. (this will work on only some drivers otherwise)
			m_useProgram->use();
            m_lightingSystem->onShaderRelink(m_useProgram->program());
            m_lightingSystem->updateAllBuffers(m_useProgram->program());
    }
}

void Painter::paint()
{
    AbstractPainter::paint();
    /*
    It depends on the current camera configuration how we need to draw the scene.
    CameraImplementation: needs to configure the cam position and can start rendering multiple times.
    RenderTechniques: decide how to render the objects ( and whether using postprocessing )
    */
    m_camera->drawScene(*m_useProgram);
}

void Painter::drawWithPostprocessing(FrameBufferObject * target)
{
    t_samplerByName sampler;

    if(m_useColor)
        drawScene(m_camera, m_useProgram, m_fboColor);
    else
        m_fboColor->clear();

    for (RenderingPass * pass : m_passes) {
        pass->applyIfActive();
    }

    sampler.clear();
    sampler["source"] = m_fboActiveBuffer;
    if(m_fboActiveBuffer == m_fboColor) {
        sampler["shadows"] = m_shadows->output();
        sampler["ssao"] = m_ssao->output();
    } else { // dont render effects
        m_fboTemp->clear();
        sampler["shadows"] = m_fboTemp;
        sampler["ssao"] = m_fboTemp;
    }

    bindSampler(sampler, *m_flush);
    // render scene to texture if needed (currently for oculus rift camera)
    m_quad->draw(*m_flush, target);
    releaseSampler(sampler);
}

void Painter::drawScene(Camera * camera, Program * program,  FrameBufferObject * fbo)
{
    fbo->bind();
    SceneTraverser traverser;
    m_lightingSystem->bindAllBuffers();
    DrawVisitor drawVisitor(program, camera->transform());
    traverser.traverse(*camera, drawVisitor);
    if (m_viewFrustumCulling) {
        CullingVisitor cullingVisitor(camera, program, camera->transform());
        traverser.traverse(*camera, cullingVisitor);
    }
    else {
        DrawVisitor drawVisitor(program, camera->transform());
        traverser.traverse(*camera, drawVisitor);
    }
    fbo->release();
}

void Painter::resize(const int width, const int height)
{
    AbstractPainter::resize(width, height);
    for (RenderingPass * pass : m_passes) {
        pass->resize(width, height);
    }
    m_camera->setViewport(width, height);

    m_fboColor->resize(width, height);
    m_fboTemp->resize(width, height);

    postShaderRelinked();
}

void Painter::selectCamera(const QString cameraName)
{
    if (m_camera->selectedImplementation() == cameraName)
        return;

    m_camera->selectImplementation(cameraName);
}

void Painter::selectRendering(const QString rendering)
{
    if (m_camera->selectedRendering() == rendering)
        return;
    m_camera->selectRenderingByName(rendering);
}

void Painter::setShading(char shader)
{
    switch(shader)
    {
        case 'p': m_useProgram = m_phong; std::printf("\nPhong Shading\n"); break;
        case 'g': m_useProgram = m_gouraud; std::printf("\nGouraud Shading\n"); break;
        case 'f': m_useProgram = m_flat; std::printf("\nFlat Shading\n"); break;
        case 'o': m_useProgram = m_gooch; std::printf("\nGooch Shading\n\n"); break;
        case 'w': m_useProgram = m_wireframe; std::printf("\nWireframe Shading\n\n"); break;
        case 's': m_useProgram = m_solidWireframe; std::printf("\nWireframeSolid Shading\n\n"); break;
        case 'r': m_useProgram = m_primitiveWireframe; std::printf("\nprimitive Wireframe Shading\n\n"); break;
        case 'n': m_useProgram = m_normals; std::printf("\nNormals\n\n"); break;
    }

    setUniforms();
}

void Painter::setFrameBuffer(int frameBuffer)
{
    switch(frameBuffer)
    {
        case 1: m_fboActiveBuffer = m_fboColor; std::printf("\nColor Buffer\n"); break;
        case 2: m_fboActiveBuffer = m_normalz->output(); std::printf("\nNormal Buffer\n"); break;
        case 3: m_fboActiveBuffer = m_shadows->output(); std::printf("\nShadows Buffer\n"); break;
        case 4: m_fboActiveBuffer = m_lightsource->output(); std::printf("\nShadowMap Buffer\n"); break;
        case 5: m_fboActiveBuffer = m_ssao->output(); std::printf("\nSSAO Buffer\n"); break;
        case 6: m_fboActiveBuffer = m_colorId->output(); std::printf("\nColorId Buffer\n"); break;
    }
}

void Painter::setEffect(int effect, bool value)
{
    switch(effect)
    {
        case 1: m_useColor = value; std::printf("\nColor toggled\n"); break;
        case 2: m_shadows->setActive(value); std::printf("\nShadow toggled\n"); break;
        case 3: m_shadowBlur->setActive(value); std::printf("\nShadow blur toggled\n"); break;
        case 4: m_ssao->setActive(value); std::printf("\nSSAO toggled\n"); break;
        case 5: m_ssaoBlur->setActive(value); std::printf("\nSSAO blur toggled\n"); break;
    }
}

void Painter::postShaderRelinked()
{
    setUniforms();
    m_shadows->setUniforms();
    m_shadowBlur->setUniforms();
    m_ssao->setUniforms();
    m_ssaoBlur->setUniforms();
}

void Painter::setViewFrustumCulling(bool viewFrustumCullingEnabled) {
    m_viewFrustumCulling = viewFrustumCullingEnabled;
}

bool Painter::isViewFrustumCullingEnabled() {
    return m_viewFrustumCulling;
}

void Painter::setBoundingBox(const glm::vec3 & llf, const glm::vec3 & urb, const glm::mat4 & transform)
{
    if (BoundingBoxPass * bbox_pass = dynamic_cast<BoundingBoxPass *>(m_boundingBox))
    {
        bbox_pass->setTransform(transform);
        bbox_pass->updateVertices(llf, urb);
    }
}

void Painter::bindSampler(
    const t_samplerByName & sampler
,   const Program & program)
{
    t_samplerByName::const_iterator i(sampler.cbegin());
    const t_samplerByName::const_iterator iEnd(sampler.cend());

    for(glm::uint slot(0); i != iEnd; ++i, ++slot)
        i.value()->bindTexture2D(program, i.key(), slot);
}

void Painter::releaseSampler(
    const t_samplerByName & sampler)
{
    t_samplerByName::const_iterator i(sampler.cbegin());
    const t_samplerByName::const_iterator iEnd(sampler.cend());

    for(; i != iEnd; ++i)
        i.value()->releaseTexture2D();

}

Camera * Painter::camera()
{
    return m_camera;
}

void Painter::sceneChanged(Group * scene)
{
    for (RenderingPass * pass : m_passes) 
    {
        pass->sceneChanged(scene);
    }
}

RenderingPass * Painter::getSharedPass()
{
    return m_colorId;
}
