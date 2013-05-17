#include "savedviews.h"


SavedViews::SavedViews(Camera * camera) 
    : AbstractNavigation(camera)
    , m_saved_views(12)
    , m_animation_progress(0)
    , m_oldMatrix(0)
{
    m_saved_views[0] = glm::lookAt(glm::vec3(0.f, 0.f,-2.f), glm::vec3(0), glm::vec3(0.f, 1.f, 0.f));
    m_saved_views[1] = glm::lookAt(glm::vec3(2.f, 0.f, 0.f), glm::vec3(0), glm::vec3(0.f, 1.f, 0.f));
    m_saved_views[2] = glm::lookAt(glm::vec3(0.f, 0.f, 2.f), glm::vec3(0), glm::vec3(0.f, 1.f, 0.f));
    m_saved_views[3] = glm::lookAt(glm::vec3(-2.f, 0.f, 0.f), glm::vec3(0), glm::vec3(0.f, 1.f, 0.f));
    for (int i = 4; i < m_saved_views.size(); i++)
    {
        m_saved_views[i] = m_saved_views[0];
    }
}

SavedViews::~SavedViews(void)
{
}

void SavedViews::saveView(int viewnr)
{
    m_saved_views[viewnr] = m_viewMatrix;
}

void SavedViews::loadView(int viewnr)
{
    m_oldMatrix = m_viewMatrix;
    m_newMatrix = m_saved_views.at(viewnr);
 
    if(!isTimerRunning())
        startTimer();
    else
        m_animation_progress = 0;
}

void SavedViews::timerEvent(QTimerEvent* event) {
    m_animation_progress += TIMER_MS/333.0f;
    if(m_animation_progress > 1) {
        m_animation_progress = 0;
        stopTimer();
        m_viewMatrix = m_newMatrix;
        updateCamera();
    } else {
        m_viewMatrix = (1-m_animation_progress) * m_oldMatrix + m_animation_progress * m_newMatrix; // TODO replace with quaternion interpolation
        updateCamera();
    }
}
