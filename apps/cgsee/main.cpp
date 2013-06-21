#include <glm/glm.hpp>

#include <QMetaType>
#include "cgsee.h"

int main(int argc, char* argv[])
{
    int result = -1;
    qRegisterMetaType<glm::vec3>("glm::vec3");
    qRegisterMetaType<glm::vec2>("glm::vec2");
    CGSee * app = new CGSee(argc, argv);
    result = app->exec();

    delete app;

    return result;
}
