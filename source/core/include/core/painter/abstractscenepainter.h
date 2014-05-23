#pragma once

// #include <glm/gtc/matrix_transform.hpp>

#include <core/core_api.h>

#include <core/painter/abstractpainter.h>

class Group;
class RenderingPass;

class CORE_API AbstractScenePainter : public AbstractPainter
{
public:
    AbstractScenePainter();
    virtual ~AbstractScenePainter();

    virtual void selectCamera(const QString cameraName) = 0;
    virtual void selectRendering(const QString rendering) = 0;
    virtual void setShading(char shader) = 0;
    virtual void setFrameBuffer(int frameBuffer) = 0;
    virtual void setEffect(int effect, bool active) = 0;
    
    virtual void postShaderRelinked() = 0;
    virtual void reloadShaders() = 0;

    // virtual void setBoundingBox(const glm::vec3 & llf, const glm::vec3 & urb) = 0;

    virtual RenderingPass *  getSharedPass() = 0;

    void assignScene(Group * scene);
    Group * retainScene();
    Group & getScene() const;
protected:
    virtual void sceneChanged(Group * scene);
    Group * m_scene;
};
