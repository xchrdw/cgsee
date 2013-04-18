
//#include <core/glformat.h>

#include "cgsee.h"

#include <gui/viewer.h>


int main(int argc, char* argv[])
{
    int result = -1;

    // Application Setup

    CGSee * app = new CGSee(argc, argv);

    // Create Viewer

    Viewer * viewer = new Viewer();
//    viewer->setWindowTitle(app->title());
  //  viewer->initialize(app->format());

    // Create Scene and Controller


    // Start

//    viewer->show();
  //  result = app->exec();

    // Clean Up

    delete viewer;
    delete app;

    return result;
}
