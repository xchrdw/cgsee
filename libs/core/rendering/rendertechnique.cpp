#include "rendertechnique.h"

RenderTechnique::RenderTechnique(Camera & camera)
    : m_camera(camera)
{
}

RenderTechnique::~RenderTechnique()
{
}

void RenderTechnique::onInvalidatedView()
{
}

void RenderTechnique::onInvalidatedViewport(const int height, const int width)
{
}

void RenderTechnique::onInvalidatedChildren()
{
}
