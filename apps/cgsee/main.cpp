
#include "cgsee.h"
#include "core/assimpwrapper.h"


int main(int argc, char* argv[])
{
    Group * group = AssimpWrapper::groupFromFile("data/suzanne.obj");
    
    int result = -1;

    CGSee * app = new CGSee(argc, argv);
    result = app->exec();

    delete app;

    return result;
}
