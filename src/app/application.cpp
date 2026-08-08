#include "application.h"

// constructor
Application::Application(int &argc, char **argv)
    : QApplication(argc, argv)
{
    // UI
    mainWindow.show();
}

// functions