#include <core/rendering/lights/directionallight.h>

#include <core/rendering/lights/abstractlight.h>

DirectionalLight::DirectionalLight(glm::vec3 direction)
{	
	setDirection(direction);
	setPosition(glm::vec3(0, 0, 0));
	setIntensity(glm::vec3(0, 0, 0));
	setFalloff(glm::vec3(0, 0, 0));
	toggleOn();
}

DirectionalLight::~DirectionalLight(){}
};