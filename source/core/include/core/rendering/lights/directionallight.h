#pragma once

#include <core/rendering/lights/abstractlight.h>

class CORE_API DirectionalLight : public AbstractLight
{
public:
	DirectionalLight(glm::vec3 direction);
	virtual ~DirectionalLight();


};