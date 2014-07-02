#include <core/camera/projection.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>

Projection::Projection(e_Type type)
:   m_type(type)
,   m_invalid(true)
,   m_viewport(glm::ivec2(1,1))//?
{

}

Projection::~Projection()
{

}
const glm::mat4 & Projection::projection() const
{
    if(m_invalid)
        update();
    return m_projection;
}


const glm::ivec2 & Projection::viewport() const
{
    return m_viewport;
}
void Projection::setViewport(const glm::ivec2 & size)
{
    setViewport(size.x, size.y);
}
void Projection::setViewport(const int width, const int height)
{
    m_viewport = glm::ivec2(width, height);
    invalidate();
}

const float Projection::fovy() const
{
    return m_fovy;
}

void Projection::setFovy(const float fovy)
{
    if(fovy == m_fovy)
        return;

    m_fovy = fovy;
    invalidate();
}

const float Projection::zNear() const
{
    return m_zNear;
}

void Projection::setZNear(const float z)
{
    if(z == m_zNear)
        return;

    m_zNear = z;
    invalidate();
}

const float Projection::zFar() const
{
    return m_zFar;
}

void Projection::setZFar(const float z)
{
    if(z == m_zNear)
        return;

    m_zFar = z;
    invalidate();
}

const float Projection::aspect() const
{
    return static_cast<float>(m_viewport.x) / static_cast<float>(m_viewport.y);
}

void Projection::invalidate() const
{
    //any actions?
    m_invalid = true;
}
void Projection::update() const
{
    if(!m_invalid)
        return;

    switch(m_type){
    case ORTHOGONAL:
        //m_projection = glm::ortho(); TODO
        break;
    case PERSPECTIVE:
    default:
        m_projection = glm::perspective(m_fovy, aspect(), m_zNear, m_zFar);
        break;
    }

    m_invalid = false;
}
