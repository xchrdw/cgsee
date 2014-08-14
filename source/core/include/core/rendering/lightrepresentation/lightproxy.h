#pragma once

#include <core/scenegraph/group.h>
#include <vector>

class AbstractLight;
class DirectionalLight;
class PointLight;
class SpotLight;

class CORE_API LightProxy : public Group
{
public:
	LightProxy(const QString & name, DirectionalLight * light);
	LightProxy(const QString & name, PointLight * light);
	LightProxy(const QString & name, SpotLight * light);
	~LightProxy();

	AbstractLight * current(); //returns a reference of the currently selected type of light

private:
	int m_current;
	std::vector<AbstractLight *> m_lights;
};