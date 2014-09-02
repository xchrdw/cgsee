//#pragma once
//
//#include <core/core_api.h>
//#include <core/rendering/defaultpass.h>
//
//#include <glm/mat4x4.hpp>
//
//#include <List>
//
//class CORE_API  VarianceShadowMappingPass : public DefaultPass
//{
//public:
//	VarianceShadowMappingPass(Camera * camera);
//	virtual ~VarianceShadowMappingPass(void);
//
//	virtual void render() override;
//	virtual void resize(const int width, const int height) override;
//
//protected:
//    virtual std::list<glm::vec2> calculateSplitPlanes(float znear, float zfar, int planesCount, float lambda);
//	virtual std::list<glm::mat4x4> calculateCropMatrices(std::list<glm::vec2> splitPlanes, Camera *lightCamera);
//
//private:
//	Program *m_lightProgram;
//
//	FrameBufferObject* m_shadowmapFBO3D;
//	FrameBufferObject* m_shadowmapFBO;
//	FrameBufferObject* m_shadowmap2FBO;
//};
//