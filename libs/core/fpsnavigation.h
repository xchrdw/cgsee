#include "flightnavigation.h"

class CGSEE_API FPSNavigation : public FlightNavigation
{
public:
    FPSNavigation(Camera * camera);
    ~FPSNavigation(void);
    
    virtual void update() override;

    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;
    
    virtual void mouseMoveEvent(QMouseEvent * event) override;
    virtual void mousePressEvent(QMouseEvent * event) override;
    virtual void mouseReleaseEvent(QMouseEvent * event) override;
    virtual void wheelEvent(QWheelEvent *event) override;
    
private:
    void yawPitch(float yawAngle, float pitchAngle);

};