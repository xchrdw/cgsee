#pragma once

#include <core/painter/abstractpainter.h>

#include <glm/vec2.hpp>

class Image;
class ScreenQuad;
class Program;

class CORE_API ImagePainter : public AbstractPainter
{
public:
    ImagePainter();
    virtual ~ImagePainter();

    // paint the scene
    virtual void paint();

    //virtual void resize(const int width, const int height);
    void assignImage(Image * image);

    void setZoomDelta(float delta);
    void setPanDelta(glm::vec2 delta);

    virtual void resize(
        const int width
        , const int height);
    
protected:
    virtual const bool initialize() override;

protected:
    void setUniforms();

protected:  
    ScreenQuad * m_quad;
    Program * m_program;

    Image * m_image;

    bool m_dirty;

    float m_zoom;
    glm::vec2 m_pan;

    glm::vec2 m_aspect;
};
