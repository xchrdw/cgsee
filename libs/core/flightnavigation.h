#include "abstractnavigation.h"

class CGSEE_API FlightNavigation : public AbstractNavigation
{
public:
	FlightNavigation(void);
	~FlightNavigation(void);
    
    void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);
	
private:
    glm::mat4 m_viewMatrix;
    

};
