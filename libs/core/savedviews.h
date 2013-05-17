#pragma once
#include "qvector.h"
#include <glm/glm.hpp>
#include "core/abstractnavigation.h"

class Camera;

class CGSEE_API SavedViews : public AbstractNavigation
{
public:
    SavedViews(Camera * camera);
    virtual ~SavedViews(void);

    void saveView(int viewnr);
    void loadView(int viewnr);

protected:
    virtual void timerEvent(QTimerEvent* event);

private:
    QVector<glm::mat4> m_saved_views;
    float m_animation_progress;
    glm::mat4 m_oldMatrix;
    glm::mat4 m_newMatrix;
};

