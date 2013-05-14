#include "abstractnavigation.h"

#ifndef flightnavigation_h
#define flightnavigation_h

class CGSEE_API FlightNavigation : public AbstractNavigation
{
public:
    FlightNavigation(Camera * camera);
    ~FlightNavigation(void);
    
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;
    
    virtual void wheelEvent(QWheelEvent *event) override;
    virtual void reset() override;
    
    void setFovy(float fovy);
    float getFovy();
    
protected:
    //Defines the view
    glm::vec3 m_eye;
    glm::vec3 m_center; 
    glm::vec3 m_up; 
    
    void updateView();
    
    //Coordinatesystem of the view
    glm::vec3 m_xView;
    glm::vec3 m_yView;
    glm::vec3 m_zView;
    
    void updateAxis();
    
    float m_fovy;
    
    //Yaw, pitch, roll: http://upload.wikimedia.org/wikipedia/commons/7/7e/Rollpitchyawplain.png
    //Rotate center around y-axis
    void yaw(float angle);
    
    //Rotate up - vector and center around x-axis
    void pitch(float angle);
    
    //Rotate up-vector around z axis
    void roll(float angle);
    
    //Move forward or backward (negative speed) to the center of the view
    void forward(float speed);
    
    void setFromMatrix(glm::mat4 view);

    int m_timesKeyPressed;
};

#endif // flightnavigation_h
