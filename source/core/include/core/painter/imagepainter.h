#pragma once

#include <core/painter/abstractpainter.h>
#include <GL/glew.h>

#include <glm/vec2.hpp>

class Image;
class ScreenQuad;
class Program;
class BufferObject;

class CORE_API ImagePainter : public AbstractPainter
{
public:
    ImagePainter();
    virtual ~ImagePainter();

    // paint the scene
    virtual void paint();
    virtual void paintGrid();

    //virtual void resize(const int width, const int height);
    void assignImage(Image * image);
    float getZoom();
    void setZoomDelta(float delta);
    void setZoomFactor(float factor);
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
    Program * m_imageProgram;
    Image * m_image;
    
    GLuint m_gridVao;
    BufferObject * m_gridVertexBO;
    Program * m_gridProgram;
    Program * m_textProgram;

    float m_pixelWidth;
    glm::vec2 m_lines;

    bool m_dirty;

    float m_zoom;
    glm::vec2 m_pan;

    glm::vec2 m_aspect;
    glm::vec2 m_viewport;
};
