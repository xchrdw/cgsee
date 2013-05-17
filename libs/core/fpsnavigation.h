#include "flightnavigation.h"

class CGSEE_API FpsNavigation : public FlightNavigation
{
public:
    FpsNavigation(Camera * camera);
    ~FpsNavigation(void);
    
    virtual void update(float delta_t) override;

    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;
    
    virtual void mouseMoveEvent(QMouseEvent * event) override;
    virtual void mousePressEvent(QMouseEvent * event) override;
    virtual void mouseReleaseEvent(QMouseEvent * event) override;
    virtual void wheelEvent(QWheelEvent *event) override;
    
private:
    void pitchYaw(float pitchAngle, float yawAngle);
    void sideward(float speed);
    void forward(float speed);
    
    void pitch(float angle);
    void yaw(float angle);
    
    bool m_inGameMode;
    
    glm::vec2 m_lastMousePosition;
    glm::vec2 m_direction;

    float m_pitchAngle;
};