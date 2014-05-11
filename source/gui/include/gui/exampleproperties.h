#pragma once

#include <gui/gui_api.h>

#include <reflectionzeug/Color.h>
#include <reflectionzeug/PropertyGroup.h>


class GUI_API ExampleProperties : public reflectionzeug::PropertyGroup
{
public:
    ExampleProperties();
    virtual ~ExampleProperties();

    std::string name() const;
    void setName(const std::string &name);

    std::string lightType() const;
    void setLightType(const std::string &type);

    int number() const;
    void setNumber(int number);

    reflectionzeug::Color color() const;
    void setColor(const reflectionzeug::Color &color);

protected:
    std::string           m_name;
    std::string           m_lightType;
    int                   m_number;
    reflectionzeug::Color m_color;
};
