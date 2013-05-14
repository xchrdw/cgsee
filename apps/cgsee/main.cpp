
#include "cgsee.h"

int main(int argc, char* argv[])
{
    int result = -1;

    CGSee * app = new CGSee(argc, argv);
    result = app->exec();

    delete app;

    return result;
}
