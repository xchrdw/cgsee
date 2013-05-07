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
    
    glm::vec3 m_position; //cameraposition
    glm::vec3 m_center; // Direction of reference point
    glm::vec3 m_up;
    
    //Rotation angle of the camera
    glm::vec3 m_xRotation;
    glm::vec3 m_yRotation;
    glm::vec3 m_zRotation;
    
    void translate(float direction);
    
    //updates Rotation after translation
    void updateRotation();
    
    
    //Yaw, pitch, roll: http://upload.wikimedia.org/wikipedia/commons/7/7e/Rollpitchyawplain.png
    //Rotation around z-axis
    void yaw(float angle);
    
    //Rotation around y-axis
    void pitch(float angle);
    
    //Rotation around x axis
    void roll(float angle);
    


};
