#include "abstractnavigation.h"

class CGSEE_API FlightNavigation : public AbstractNavigation
{
public:
	FlightNavigation(void);
	~FlightNavigation(void);
    
    virtual void keyPressEvent(QKeyEvent *event);
	virtual void keyReleaseEvent(QKeyEvent *event);
    virtual void reset();
    
    virtual const glm::mat4 viewMatrix();
    virtual const glm::mat4 viewMatrixInverted();
	
private:
    glm::mat4 m_viewMatrix;
    
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
    
    //Yaw, pitch, roll: http://upload.wikimedia.org/wikipedia/commons/7/7e/Rollpitchyawplain.png
    //Rotate center around y-axis
    void yaw(float angle);
    
    //Rotate up - vector and center around x-axis
    void pitch(float angle);
    
    //Rotate up-vector around z axis
    void roll(float angle);
    
    //Move forward or backward to the center of the view
    void translate(float direction);
};
