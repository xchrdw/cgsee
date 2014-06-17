#pragma once

#include <core/painter/abstractpainter.h>

class Image;
class ScreenQuad;
class Program;
class FrameBufferObject;

class CORE_API ImagePainter : public AbstractPainter
{
public:
    ImagePainter();
    virtual ~ImagePainter();

    // paint the scene
    virtual void paint();

    //virtual void resize(const int width, const int height);
    void assignImage(Image * image);
    
protected:
    virtual const bool initialize() override;

protected:
    void setUniforms();

    void drawScene(Camera * camera, Program * program, FrameBufferObject * fbo);

protected:  
    ScreenQuad * m_quad;
    Program * m_program;

    Image * m_image;

    bool m_image_bound;
};
