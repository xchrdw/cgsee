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
    
    glm::vec3 m_center;

};
