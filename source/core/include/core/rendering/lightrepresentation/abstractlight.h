#pragma once

#include <core/core_api.h>
#include <core/scenegraph/group.h>

class LightcollectVisitor;
class LightingSystem;

class CORE_API AbstractLight : public Group
{

public:
	AbstractLight(const QString & name, const glm::vec3 & intensity);
	virtual ~AbstractLight();

	void toggle();
	void setIntensity(const glm::vec3 & rgbVector);
	glm::vec3 intensity() const;

	virtual void saveLightData(LightingSystem & manager, const glm::mat4 & transform) = 0;
	virtual void draw(const Program & program, const glm::mat4 & transform) = 0;
	virtual const AxisAlignedBoundingBox boundingBox() const = 0;
	virtual const AxisAlignedBoundingBox boundingBox(glm::mat4 transform) const = 0;

protected: 
	glm::vec3 m_intensity;
	bool m_enable;
};