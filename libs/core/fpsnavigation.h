#include "flightnavigation.h"

class CGSEE_API FPSNavigation : public FlightNavigation
{
public:
    FPSNavigation(Camera * camera);
    ~FPSNavigation(void);
    
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);
    
    virtual void mouseMoveEvent(QMouseEvent * event);
    virtual void mousePressEvent(QMouseEvent * event);
    virtual void mouseReleaseEvent(QMouseEvent * event);
    virtual void wheelEvent(QWheelEvent *event);
    
private:
    void yawPitch(float yawAngle, float pitchAngle);

};