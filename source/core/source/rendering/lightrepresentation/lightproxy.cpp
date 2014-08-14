#include <core/rendering/lightrepresentation/lightproxy.h>
#include <core/rendering/lightrepresentation/directionallight.h>
#include <core/rendering/lightrepresentation/pointlight.h>
#include <core/rendering/lightrepresentation/spotlight.h>

LightProxy::LightProxy(const QString & name, DirectionalLight * light)
: m_lights(std::vector<AbstractLight *>(3)),
m_current(0),
Group(name)
{
	m_lights.at(0) = light;
}

LightProxy::LightProxy(const QString & name, PointLight * light)
: m_lights(std::vector<AbstractLight *>(3)),
m_current(1),
Group(name)
{
	m_lights.at(1) = light;
}

LightProxy::LightProxy(const QString & name, SpotLight * light)
: m_lights(std::vector<AbstractLight *>(3)),
m_current(2),
Group(name)
{
	m_lights.at(2) = light;
}

LightProxy::~LightProxy()
{

}

AbstractLight * LightProxy::current()
{
	return m_lights.at(m_current);
}