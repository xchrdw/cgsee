//const int ShadowMapSize = 2048;
//
//const glm::mat4 VarianceShadowMappingPass::biasMatrix(
//	0.5, 0.0, 0.0, 0.0,
//	0.0, 0.5, 0.0, 0.0,
//	0.0, 0.0, 0.5, 0.0,
//	0.5, 0.5, 0.5, 1.0
//	);
//
//VarianceShadowMappingPass::VarianceShadowMappingPass(Camera * camera)
//: DefaultPass(camera)
//, m_lightProgram(new Program())
//, m_blurh(new Program())
//, m_blurv(new Program())
// {
//	m_lightProgram->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/shadows/vsm_light.frag"));
//    m_lightProgram->attach(new FileAssociatedShader(GL_GEOMETRY_SHADER, "data/shadows/vsm_light.geom"));
//	m_lightProgram->attach(new FileAssociatedShader(GL_VERTEX_SHADER, "data/shadows/vsm_light.vert"));
//
//	m_program->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/shadows/vsm_shadow.frag"));
//	m_program->attach(new FileAssociatedShader(GL_VERTEX_SHADER, "data/shadows/vsm_shadow.vert"));
//
//	m_blurh->attach(new FileAssociatedShader(GL_VERTEX_SHADER, "data/screenquad.vert"));
//	m_blurh->attach(new FileAssociatedShader(GL_GEOMETRY_SHADER, "data/shadows/screenquad_layered.geom"));
//	m_blurh->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/shadows/gauss_blur_5_h_layered.frag"));
//
//	m_blurv->attach(new FileAssociatedShader(GL_VERTEX_SHADER, "data/screenquad.vert"));
//	m_blurv->attach(new FileAssociatedShader(GL_GEOMETRY_SHADER, "data/shadows/screenquad_layered.geom"));
//	m_blurv->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/shadows/gauss_blur_5_v_layered.frag"));
//
//	m_shadowmapFBO3D = new FrameBufferObject(GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0, 2, true);
//	m_shadowmapFBO3D->resize(ShadowMapSize, ShadowMapSize);
//
//	m_blurHFBO = new FrameBufferObject(GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0, 2, false);
//	m_blurHFBO->resize(ShadowMapSize, ShadowMapSize);
//
//	m_blurVFBO = new FrameBufferObject(GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0, 2, false);
//	m_blurVFBO->resize(ShadowMapSize, ShadowMapSize);
//}
//
//VarianceShadowMappingPass::~VarianceShadowMappingPass()
//{
//	delete(m_lightProgram);
//	delete(m_shadowmapFBO3D);
//}
//
//void VarianceShadowMappingPass::render()
//{
//    AxisAlignedBoundingBox bb = m_camera->asGroup()->boundingBox();
//
//    Camera lightCamera = Camera();
//    lightCamera.setFovy(90.0);
//    lightCamera.setZNear(m_camera->zNear());
//    lightCamera.setZFar(m_camera->zFar());
//    lightCamera.setViewport(glm::ivec2(ShadowMapSize, ShadowMapSize));
//    lightCamera.setView(glm::lookAt(glm::vec3(4.0f, 5.5f, 6.0f) + bb.center(),
//        bb.center(), glm::vec3(0.0f, 1.0f, 0.0f)));
//    lightCamera.update();
//
//    Camera light2Camera = Camera();
//    light2Camera.setFovy(90.0);
//    light2Camera.setZNear(m_camera->zNear());
//    light2Camera.setZFar(m_camera->zFar());
//    light2Camera.setViewport(glm::ivec2(ShadowMapSize, ShadowMapSize));
//    light2Camera.setView(glm::lookAt(glm::vec3(-2.0f, 5.5f, -4.0f) + bb.center(),
//        bb.center(), glm::vec3(0.0f, 1.0f, 0.0f)));
//    light2Camera.update();
//
//	glm::mat4x4 *biasLightViewProjection = new glm::mat4x4[2];
//	biasLightViewProjection[0] = biasMatrix * lightCamera.transform();
//	biasLightViewProjection[1] = biasMatrix * light2Camera.transform();
//
//    glm::mat4x4 *lightViewProjection = new glm::mat4x4[2];
//    lightViewProjection[0] = lightCamera.transform();
//    lightViewProjection[1] = light2Camera.transform();
//
//    m_lightProgram->setUniform("lightTransforms", 2, lightViewProjection[0]);
//    m_lightProgram->setUniform("inverseViewProjection", m_camera->transformInverse());
//    drawScene(m_camera, m_lightProgram, m_shadowmapFBO3D);
//
//	ScreenQuad screenQuad;
//	m_shadowmapFBO3D->bindTexture3D(*m_blurv, "source", 0);
//	m_blurv->setUniform("viewport", glm::ivec2(ShadowMapSize, ShadowMapSize));
//	screenQuad.draw(*m_blurv, m_blurVFBO);
//
//	m_blurVFBO->bindTexture3D(*m_blurh, "source", 0);
//	m_blurh->setUniform("viewport", glm::ivec2(ShadowMapSize, ShadowMapSize));
//	screenQuad.draw(*m_blurh, m_shadowmapFBO3D);
//
//	m_shadowmapFBO3D->bindTexture3D(*m_program, "shadowmap3D", 0);
//	m_program->setUniform("inverseViewProjection", m_camera->transformInverse());
//	m_program->setUniform("biasLightViewProjection", 2, biasLightViewProjection[0]);
//	drawScene(m_camera, m_program, m_fbo);
//
//    delete[] biasLightViewProjection;
//    delete[] lightViewProjection;
//}
//
//void VarianceShadowMappingPass::resize(const int width, const int height)
//{
//	DefaultPass::resize(width, height);
//    m_camera->update();
//}