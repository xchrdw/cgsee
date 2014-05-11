#include <gui/exampleproperties.h>


ExampleProperties::ExampleProperties()
: reflectionzeug::PropertyGroup("ExampleProperties")
, m_name("unnamed")
, m_lightType("PointLight")
, m_number(3)
, m_color(0, 255, 0, 255)
{
    addProperty<std::string>("Name", this, &ExampleProperties::name, &ExampleProperties::setName);

    std::vector<std::string> lightTypes = {
        "PointLight", "AreaLight", "SpotLight", "DirectedLight", "AmbientLight"
    };
    addProperty<std::string>("LightType", this, &ExampleProperties::lightType, &ExampleProperties::setLightType);
    property<std::string>("LightType")->setChoices(lightTypes);

    addProperty<int>("Number", this, &ExampleProperties::number, &ExampleProperties::setNumber);
    property<int>("Number")->setRange(2, 10);

    addProperty<reflectionzeug::Color>("Color", this, &ExampleProperties::color, &ExampleProperties::setColor);
}

ExampleProperties::~ExampleProperties()
{
}

std::string ExampleProperties::name() const
{
    return m_name;
}

void ExampleProperties::setName(const std::string &name)
{
    m_name = name;
}

std::string ExampleProperties::lightType() const
{
    return m_lightType;
}

void ExampleProperties::setLightType(const std::string &type)
{
    m_lightType = type;
}

int ExampleProperties::number() const
{
    return m_number;
}

void ExampleProperties::setNumber(int number)
{
    m_number = number;
}

reflectionzeug::Color ExampleProperties::color() const
{
    return m_color;
}

void ExampleProperties::setColor(const reflectionzeug::Color &color)
{
    m_color = color;
}
